#include <iostream>

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>

#include "Ray.h"
#include "Screen.h"
#include "Scene.h"

#include "IOfunctions.h"

//#define DEBUG_OUTPUTALPHA
//#define DEBUG_OUTPUTDIST
//#define DEBUG_OUTPUTPOS
//#define DEBUG_OUTPUTSAMPLES

////SCENE SETUP////
const int IMG_W = 250;
const int IMG_H = 250;

const double SCN_MAXDIST = 5000;
const double SCN_MAXRAYDIST = 5000;
const double SCN_RAYBIAS = 0.1;
const int SCN_MAXDEPTH = 1;

const int SHD_MINSAMPLES = 2;
const int SHD_MAXSAMPLES = 2;
const double SHD_MINVARIANCE = 0.1;

const int SAMP_MINSAMPLES = 4;
const int SAMP_MAXSAMPLES = 256;
const double SAMP_MINVARIANCE = 0.1;

const bool SCN_JITTER = true;

unsigned int g_triIntersections = 0;
unsigned int g_boxIntersections = 0;

////END OF SCENE SETUP////

const double invRandMax2 = (2.0 / (double)RAND_MAX);
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
	return (rand() * invRandMax2) - 1.0;
}

Pixel renderPixel(Ray& ray, Scene& scene, int depth=0)
{
	double intersectDist;
	double shortestDist;

	if (depth > 0)
		shortestDist = SCN_MAXDIST;
	else
		shortestDist = SCN_MAXRAYDIST;

	Tri* closestTri = &(scene.meshes[0].triList[0]);

	for (int obj = 0; obj < scene.numberOfMeshes(); obj++)
	{
		Mesh& mesh = scene.meshes[obj];

		//skip bounding box intersection test if mesh has less than 4 trianges
		intersectDist = mesh.boundingBox.intersect(ray);
		g_boxIntersections++;



		if (mesh.boundingBox.contains(ray.p) || (intersectDist > 0.0 && intersectDist < shortestDist))
		{
			for (int tri = 0; tri < mesh.numberOfTris(); tri++)
			{
				
				intersectDist = mesh.triList[tri].intersect(ray);
				g_triIntersections++;

				if (intersectDist > 0 && intersectDist < shortestDist)
				{
					shortestDist = intersectDist;
					
					closestTri = &(mesh.triList[tri]);
				}
			}
		}
	}

#ifdef DEBUG_OUTPUTDIST
	shortestDist *= 0.01;
	return Pixel(shortestDist, shortestDist, shortestDist, 1);
#endif

#ifdef DEBUG_OUTPUTALPHA
	if (shortestDist < SCN_MAXDIST)
		return Pixel(1, 1, 1, 1);
	else
		return Pixel(0, 0, 0, 0);
#endif

#ifdef DEBUG_OUTPUTPOS
	Vec temppos = ray.p + (ray.dir * shortestDist);
	return Pixel(abs(temppos.x*0.1), abs(temppos.y*0.1), abs(temppos.z*0.1), 1);
#endif

	if (depth < SCN_MAXDEPTH)
	{
		if (shortestDist < SCN_MAXDIST)
		{
			Pixel outPixel;
			Ray indirectRay;
			indirectRay.setPos(ray.p + (ray.dir * shortestDist) + ((*closestTri).n * SCN_RAYBIAS));

			for (int i = 0; i < SHD_MAXSAMPLES; i++)
			{
				indirectRay.setDir(randfneg(), randfneg(), randfneg());
				indirectRay.dir.normalize();

				while (indirectRay.dir.dot((*closestTri).n) <= 0.0)
				{
					indirectRay.setDir(randfneg(), randfneg(), randfneg());
					indirectRay.dir.normalize();
				}
				outPixel += renderPixel(indirectRay, scene, depth + 1);
			}
			outPixel /= SHD_MAXSAMPLES;
			outPixel.a = 1.0;
			return outPixel;
		}
	}
	else//if depth == SCN_MAXDEPTH (last bounce)
	{
		if (shortestDist < SCN_MAXDIST)
			return Pixel(0, 0, 0, 1);
	}

	return Pixel(1, 1, 1, 0);
}

int main()
{
	srand(12345);

	string objfile = "lamp";
	Scene myScene = readObj("D:/Users/Yegor/Desktop/raytracer/objects/"+objfile+".obj");
	//myScene.printInfo();

	//myScene.buildBboxHierarchy();

	Screen myScreen(IMG_W, IMG_H);
	Ray primaryRay;
	Vec dir;
	primaryRay.setPos(0, 0, 0);
	//double shortestDist = SCN_MAXDIST;
	//double intersectDist = 0;

	float sx, sy;

	//bool hit = false;
	//int cl = 0;
	//double curvature = 0;
	cout << endl << "Rendering.. " << endl;

	Pixel currentPixel;
	Pixel renderedPixel;

	Pixel variance;
	int varianceCombo = 0;
	double ss;

	clock_t begin = clock();

	//for each row
	for (int y = 0; y < IMG_H; y++ )
	{
		cout << y << endl;
		//for each pixel
		for (int x = 0; x < IMG_W; x++ )
		{
			varianceCombo = 0;
			//for each sample
			double s = 1.0;
			while (varianceCombo < SAMP_MINSAMPLES && s <= SAMP_MAXSAMPLES)
			{
				sx = ((x + randfneg()*0.5 - IMG_W * 0.5) / (float)IMG_H) * 2;
				sy = ((y + randfneg()*0.5 - IMG_H * 0.5) / (float)IMG_H) * -2;

				primaryRay.setDir(sx* 0.6, sy*0.6, -1.0);
				primaryRay.dir.normalize();

				variance = currentPixel;

				renderedPixel = renderPixel(primaryRay, myScene);
				renderedPixel *= 1.0 / s;
				currentPixel *= (s - 1.0) / s;
				currentPixel += renderedPixel;

				variance -= currentPixel;

				if (variance.magnitude() < SAMP_MINVARIANCE)
					varianceCombo++;
				else
					varianceCombo = 0;

				s++;
			}

#ifdef DEBUG_OUTPUTSAMPLES
			double samplesUsed = s / (double)SAMP_MAXSAMPLES;
			currentPixel.setColor(samplesUsed, samplesUsed, samplesUsed);
			currentPixel.setAlpha(1);
#endif

			myScreen.setPixel(x, y, currentPixel);

			//crtl+d early exit & dump pixels
			if (_kbhit() && _getch() == 4)
			{
				cout << endl << "Render Cancelled.." << endl;
				goto end_dump;
			}

		}
	}
	
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
	cout << g_boxIntersections << " box intersections tested." << endl;
	//myScreen.invertRGB();
	//myScreen.constantAlpha();
	writePPM(myScreen, "D:/Users/Yegor/Desktop/raytracer/"+objfile+".ppm", "rgba");
	//myScene.printInfo();
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

int *myPointer = &myVar; myPointer points to myVar


int* myPointer;
myPointer = &y;
x = *myPointer;

same as x = y;

*/