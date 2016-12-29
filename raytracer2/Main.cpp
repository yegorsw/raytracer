#include <iostream>

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <thread>

#include "Ray.h"
#include "Screen.h"
#include "Scene.h"
#include "Pixel.h"

#include "IOfunctions.h"
#include "globals.h"

////SCENE SETUP//// - 25.5 seconds
const int IMG_W = 600;
const int IMG_H = 600;

const double SCN_MAXDIST = 5000;
const double SCN_RAYBIAS = 0.001;
const int SCN_MAXDEPTH = 1;

const int SHD_MAXSAMPLES = 4;

const int SAMP_MINSAMPLES = 4;
const int SAMP_MAXSAMPLES = 512;
const double SAMP_MAXVARIANCE = 1;

const int NUM_THREADS = 4;
////END OF SCENE SETUP////

unsigned int g_triIntersections = 0;
unsigned int g_bboxIntersections = 0;

const double invRandMax = (1.0 / (double)RAND_MAX);

using namespace std;

double length(double a, double b)
{
	return sqrt(a * a + b * b);
}

double randf()
{
	return rand() * invRandMax;
}

double randfneg()
{
	return (rand() * invRandMax * 2.0) - 1.0;
}

Pixel renderPixel(Ray& ray, Scene& scene, int depth = 0)
{
	double intersectDist;
	double shortestDist = SCN_MAXDIST;
	
	Tri* closestTri;
	bool intersected;

	intersected = scene.intersect(ray, closestTri, shortestDist);
	
#ifdef DEBUG_OUTPUTDIST
	return Pixel(shortestDist, shortestDist, shortestDist, 1);
#endif

#ifdef DEBUG_OUTPUTNORMALS
	if (intersected)
		return Pixel(closestTri->n.p[0], closestTri->n.p[1], closestTri->n.p[2], 1);
	return Pixel(0, 0, 0, 0);
#endif

#ifdef DEBUG_OUTPUTALPHA
	if (intersected && shortestDist < SCN_MAXDIST)
		return Pixel(1, 1, 1, 1);
	else
		return Pixel(0, 0, 0, 0);
#endif

#ifdef DEBUG_OUTPUTPOS
	Vec temppos = ray.p + (ray.dir * shortestDist);
	return Pixel(abs(temppos[0]*0.1), abs(temppos[1]*0.1), abs(temppos[2]*0.1), 1);
#endif

	if (intersected){
		if (depth < SCN_MAXDEPTH && closestTri->mtl->hasDiffuse())
		{
			Pixel outPixel;
			Pixel nextBounce;
			Ray indirectRay;
			indirectRay.setPos(ray.pos + (ray.dir * shortestDist) + (closestTri->n * SCN_RAYBIAS));

			int numsamples = max(SHD_MAXSAMPLES / (depth + 1), 1);
			for (int i = 0; i < numsamples; i++)
			{
				do indirectRay.setDir(randfneg(), randfneg(), randfneg());
				while (indirectRay.dir.dot(indirectRay.dir) > 1.0);

				indirectRay.dir.normalize();

				if (indirectRay.dir.dot(closestTri->n) < 0.0)
					indirectRay.dir = -indirectRay.dir;
						
				nextBounce = renderPixel(indirectRay, scene, depth + 1);
				nextBounce.color *= closestTri->mtl->Kd;
				nextBounce.color *= indirectRay.dir.dot(closestTri->n);
				nextBounce.color += closestTri->mtl->Ka;
				
				outPixel.color += nextBounce.color;
			}
			outPixel /= numsamples;
			outPixel.a = 1.0;
			return outPixel;
		}
		else//if depth == SCN_MAXDEPTH (last bounce)
		{
			return Pixel(closestTri->mtl->Ka, 1);
		}
	}
	else//!intersected, return sky color
	{
		return Pixel(scene.skyColor.Ka, 0);
	}
}

void renderThread(Scene& scene, Screen& screen, int screenx, int screeny)
{
	int sl = 0;
	double samples;
	double s = 1;
	Pixel samplePixel, outPixel, variance;

	Ray primaryRay;

	int varianceCombo = 0;

	double dirx, diry;

	while (varianceCombo > SAMP_MINSAMPLES && sl < SAMP_MAXSAMPLES)
	{
		samples = pow(2, sl);
		for (double x = 0; x < samples; x++)
		{
			for (double y = 0; y < samples; y++)
			{
				dirx = ((x + randfneg() - IMG_W * 0.5) / (double)IMG_H) * 2;
				diry = ((y + randfneg() - IMG_H * 0.5) / (double)IMG_H) * -2;
				primaryRay.setDir(dirx, diry, -2);
				primaryRay.dir.normalize();

				variance = outPixel;
				samplePixel = renderPixel(primaryRay, scene);

				outPixel *= (s - 1.0) / s;

				samplePixel /= s;

				outPixel += samplePixel;

				variance -= outPixel;

				s++;

				if (variance.magnitude() < SAMP_MAXVARIANCE)
					varianceCombo++;
				else
					varianceCombo = 0;
			}
		}
		sl++;
	}
	screen.setPixel(screenx, screeny, outPixel);
}

int main()
{
	srand(12345);

	string objfile = "lampglow";
	string rootdir = "C:/Users/Yegor-s/Desktop/raytracer/";
	Scene myScene = readObj(rootdir + "objects/" + objfile + ".obj");
//	myScene.skyColor.Ka = { 0.1, 0.2, 0.4 };
	cout << myScene.numberOfTris() << " triangles in scene" << endl;
	cout << "Building BVH tree" << endl;
	myScene.buildBboxHierarchy();
//	myScene.findEmptyChildren();

	Screen myScreen(IMG_W, IMG_H);
	Ray primaryRay;
	primaryRay.setPos(0, 0, 0);

	double sx, sy, rx, ry;
	
	cout << endl << "Rendering.. " << endl;

	Pixel currentPixel;
	Pixel renderedPixel;

	Pixel variance;
	int varianceCombo = 0;

	clock_t begin = clock();
	clock_t percent = clock();

	//for each row
	for (int y = 0; y < IMG_H; y++ )
	{

		//percentage output
		if (clock() - percent > 1000)
		{
			percent = clock();
			cout << (int)((double)y * 100.0 / (double)IMG_H) << '%' << endl;
		}

		//for each pixel
		for (int x = 0; x < IMG_W; x++ )
		{
			varianceCombo = 0;
			//for each sample
			double s = 1.0;
			while (varianceCombo < SAMP_MINSAMPLES && s <= SAMP_MAXSAMPLES)
			{
				rx = randfneg()*0.5;
				ry = randfneg()*0.5;
//				rx = rx * rx * sign(rx) * 0.5;
//				rx = ry * ry * sign(ry) * 0.5;

				sx = ((x + rx - IMG_W * 0.5) / (double)IMG_H) * 2;
				sy = ((y + ry - IMG_H * 0.5) / (double)IMG_H) * -2;

				primaryRay.setDir(sx* 0.6, sy*0.6, -1.0);
				primaryRay.dir.normalize();

				variance = currentPixel;

				renderedPixel = renderPixel(primaryRay, myScene);
				renderedPixel.color.clamp();
				renderedPixel *= 1.0 / s;

				currentPixel.color.clamp();
				currentPixel *= (s - 1.0) / s;
				currentPixel += renderedPixel;

				variance -= currentPixel;

				if (variance.magnitude() < SAMP_MAXVARIANCE)
					varianceCombo++;
				else
					varianceCombo = 0;



				s++;
			}//end for each sample
			

#ifdef DEBUG_OUTPUTSAMPLES
			double samplesUsed = s / (double)SAMP_MAXSAMPLES;
			currentPixel.setColor(samplesUsed, samplesUsed, samplesUsed);
			currentPixel.setAlpha(1);
#endif

#ifdef DEBUG_OUTPUTINTERSECTIONS
			currentPixel.setColor(g_triIntersections, g_bboxIntersections, currentPixel.a);

			currentPixel.setAlpha(1);
			g_triIntersections = 0;
			g_bboxIntersections = 0;
#endif
			myScreen.setPixel(x, y, currentPixel);

			//crtl+d early exit & dump pixels
			if (_kbhit() && _getch() == 4)
			{
				cout << endl << "Render Cancelled.." << endl;
				goto end_dump;
			}

		}//end for each pixel
	}//end for each row
	
	//crtl+d early exit & dump pixels goto point
	end_dump:
	
	clock_t end = clock();
	double rendertime = double(end - begin) / CLOCKS_PER_SEC;
	cout << endl << "Render time: " << rendertime<< endl;
	cout << g_triIntersections << " triangle intersections tested." << endl;
	cout << g_bboxIntersections << " box intersections tested." << endl;
	//myScreen.invertRGB();
	//myScreen.constantAlpha();

#ifdef DEBUG_OUTPUTINTERSECTIONS
	myScreen.normalizeValues();
#endif
#ifdef DEBUG_OUTPUTDIST
	myScreen.normalizeValues();
#endif

	myScreen.applyGamma(2.2);
	writePPM(myScreen, rootdir + objfile + ".ppm", "rgba");
	//myScene.printToConsole();
}


/*
double l = length(sx, sy);
sx = (sx * (1 - sqrt(1 - l * l))) * curvature + sx * (1.0 - curvature);
sy = (sy * (1 - sqrt(1 - l * l))) * curvature + sy * (1.0 - curvature);
*/

/*
int *myPointer; 
create a pointer

*myPointer;
dereference a pointer

(*myPointer).setRGBValues()

if there is a datatype preceding the '*', '*' means "pointer type", otherwise '*' means dereference

myPointer = &myVar; 
myPointer points to myVar

OR

int *myPointer = &myVar; 
myPointer points to myVar


int* myPointer;
myPointer = &y;
x = *myPointer;

same as x = y;

*/
//test1
//asdfasdhhghgh 
