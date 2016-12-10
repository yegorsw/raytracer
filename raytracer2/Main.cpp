#include <iostream>

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>

#include "Ray.h"
#include "Screen.h"
#include "Scene.h"

#include "IOfunctions.h"
#include "globals.h"

////SCENE SETUP//// - 25.5 seconds
const int IMG_W = 300;
const int IMG_H = 300;

const double SCN_MAXDIST = 5000;
const double SCN_MAXRAYDIST = 5000;
const double SCN_RAYBIAS = 0.01;
const int SCN_MAXDEPTH = 1;

const int SHD_MAXSAMPLES = 4;

const int SAMP_MINSAMPLES = 10;
const int SAMP_MAXSAMPLES = 4000;
const double SAMP_MINVARIANCE = 0.001;
////END OF SCENE SETUP////

unsigned int g_triIntersections = 0;
unsigned int g_bboxIntersections = 0;

const double invRandMax = (1.0 / (double)RAND_MAX);

using namespace std;

int sign(float n)
{
	return n >= 0 ? 1 : -1;
}

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
	double shortestDist;

	if (depth > 0)
		shortestDist = SCN_MAXDIST;
	else
		shortestDist = SCN_MAXRAYDIST;
	
	Tri* closestTri;
	bool intersected;

	intersected = scene.intersect(ray, closestTri, shortestDist);

#ifdef DEBUG_OUTPUTDIST
	return Pixel(shortestDist, shortestDist, shortestDist, 1);
#endif

#ifdef DEBUG_OUTPUTALPHA
	if (intersected && shortestDist < SCN_MAXDIST)
		return Pixel(1, 1, 1, 1);
	else
		return Pixel(0, 0, 0, 0);
#endif

#ifdef DEBUG_OUTPUTPOS
	Vec temppos = ray.p + (ray.dir * shortestDist);
	return Pixel(abs(temppos.x*0.1), abs(temppos.y*0.1), abs(temppos.z*0.1), 1);
#endif

	if (intersected){
		if (depth < SCN_MAXDEPTH && closestTri->mtl->hasDiffuse())
		{
			if (shortestDist < SCN_MAXDIST)
			{
				Pixel outPixel;
				Ray indirectRay;
				indirectRay.setPos(ray.p + (ray.dir * shortestDist) + ((*closestTri).n * SCN_RAYBIAS));

				for (int i = 0; i < max(SHD_MAXSAMPLES/(depth+1), 1); i++)
				{
					indirectRay.setDir(randfneg(), randfneg(), randfneg());
					indirectRay.dir.normalize();

					if (indirectRay.dir.dot((*closestTri).n) < 0.0)
						indirectRay.dir = -indirectRay.dir;
					
					Pixel nextBounce = renderPixel(indirectRay, scene, depth + 1);

					nextBounce.color *= closestTri->mtl->Kd;
					nextBounce.color += closestTri->mtl->Ka;
					
					outPixel += nextBounce;
				}
				outPixel /= SHD_MAXSAMPLES;
				outPixel.a = 1.0;
				return outPixel;
			}
		}
		else//if depth == SCN_MAXDEPTH (last bounce)
		{
			if (shortestDist < SCN_MAXDIST)
			{
				int val = shortestDist * 100.0;
				val = 1.0 - (1.0 / 
					((val + 1) * (val + 1))
					);
				val = clamp(val, 0.0, 1.0);

				return Pixel(closestTri->mtl->Ka, 1);
			}
		}
	}

	//no intersection, return black
	return Pixel(0, 0, 0, 0);
}

int main()
{
	srand(12345);

	string objfile = "simpleboxesshaded";
	Scene myScene = readObj("D:/Users/Yegor/Desktop/raytracer/objects/"+objfile+".obj");
	cout << myScene.numberOfTris() << endl;

//	myScene.buildBboxHierarchy();
//	myScene.findEmptyChildren();

	Screen myScreen(IMG_W, IMG_H);
	Ray primaryRay;
	Vec dir;
	primaryRay.setPos(0, 0, 0);
	//double shortestDist = SCN_MAXDIST;
	//double intersectDist = 0;

	float sx, sy, rx, ry;

	//bool hit = false;
	//int cl = 0;
	//double curvature = 0;
	cout << endl << "Rendering.. " << endl;

	Pixel currentPixel;
	Pixel renderedPixel;

	Pixel variance;
	int varianceCombo = 0;

	clock_t begin = clock();

	//for each row
	for (int y = 0; y < IMG_H; y++ )
	{

		//percentage output
		if (!(y % 15))
		{
			cout << (int)((float)y * 100.0 / (float)IMG_H) << endl;
		}

		//for each pixel
		for (int x = 0; x < IMG_W; x++ )
		{
			varianceCombo = 0;
			//for each sample
			double s = 1.0;
			while (varianceCombo < SAMP_MINSAMPLES && s <= SAMP_MAXSAMPLES)
			{
				rx = randfneg();
				ry = randfneg();
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

				if (variance.magnitude() < SAMP_MINVARIANCE)
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

	/*
	Ray testRay;
	testRay.setPos(Vec(0, -0.1, -15.5));
	testRay.setDir(0, 1, 0);
	*/
	
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

	//myScreen.applyGamma(2.2);
	writePPM(myScreen, "D:/Users/Yegor/Desktop/raytracer/"+objfile+".ppm", "rgba");
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