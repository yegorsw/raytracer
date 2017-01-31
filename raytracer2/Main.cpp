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
#include "MtlLib.h"
#include "ymath.h"

#include "IOfunctions.h"
#include "globals.h"

using namespace std;

Pixel renderPixel(Ray& ray, Scene& scene, int depth = 0)
{
	double intersectDist;
	double shortestDist = SCN_MAXDIST;
	
	Tri* closestTri;
	bool intersected;

	int numTriIntersections = 0;
	int numBoxIntersections = 0;
	
	intersected = scene.intersect(ray, closestTri, shortestDist, numBoxIntersections);
	
#ifdef DEBUG_OUTPUTDIST
	return Pixel(Color(shortestDist/SCN_MAXDIST), 1);
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

#ifdef DEBUG_OUTPUTINTERSECTIONS
	return Pixel((double)numBoxIntersections, (double)numBoxIntersections / ((double)depth + 1.0), 0.0, 1.0);
#endif

	Pixel outPixel(0.0, 0.0, 0.0, 0.0);

	if (intersected){

		vector<Shader*> &shaderstack = closestTri->mtl->shaderstack;

		//light scatter block
		if (depth < SCN_MAXDEPTH && closestTri->mtl->scatterslight)
		{			
			bool traceNextBounce = false;

			Ray indirectRay;
			indirectRay.setPos(ray.pos + (ray.dir * shortestDist) + (closestTri->n * SCN_RAYBIAS));

			Pixel nextBounce;
			Color remainingLight = 1.0;
			Color scatterColor;

			Vec normal = (closestTri->n0 * ray.b0) + (closestTri->n1 * ray.b1) + (closestTri->n2 * ray.b2);
			int numsamples;
			double alpha = 0;

			for (vector<Shader*>::iterator s = closestTri->mtl->shaderstack.begin();
				s != closestTri->mtl->shaderstack.end(); 
				s++
				)
			{
				Shader*& shader = *s;
				if (shader->scatterslight)
				{
					int numsamples = depth > 0 ? 1 : shader->samples;
					for (int i = 0; i < numsamples; i++)
					{
						shader->scatterInRandomDirection(indirectRay, ray, normal);
						nextBounce = renderPixel(indirectRay, scene, depth + 1);
						nextBounce.color *= shader->getScatterColor(ray.dir, indirectRay.dir, normal);
						
						outPixel.color += nextBounce.color;
					}

					outPixel /= numsamples;

					scatterColor = 
					alpha = (1.0 - alpha) * shader->hitChance();

				}
				outPixel.a = 1.0;
			}
			return outPixel;
		}//end light scatter block

		 //light emission loop
		if (closestTri->mtl->emitslight)
		{
			Ray indirectRay;
			Vec normal;
			for (vector<Shader*>::iterator s = shaderstack.begin(); s != shaderstack.end(); s++)
			{
				if ((**s).emitslight)
				{
					outPixel.color += ((**s).getScatterColor(ray.dir, indirectRay.dir, normal) * (**s).hitChance());
					outPixel.a += (**s).hitChance();
				}
			}//end light emission loop
			//clamp alpha
			outPixel.a = outPixel.a < 1.0 ? outPixel.a : 1.0;

		}
	}
	else //no intersections, return sky color
	{
		outPixel.color = scene.skyColor;
	}

	return outPixel;
}

void renderThread(Scene& scene, Screen& screen, int screenx, int screeny)
{
	int s = 1;
	Pixel samplePixel, outPixel, prevPixel;

	Ray primaryRay;

	int varianceCombo = 0;

	double dirx, diry;

	while (varianceCombo < SAMP_MINSAMPLES && s <= SAMP_MAXSAMPLES)
	{
		dirx = ((screenx + (haltonRand(s, 2) - 0.5) * (SAMP_MAXSAMPLES>1 ? 1 : 0) - 0.5 - IMG_W * 0.5) / (double)IMG_H) * 2;
		diry = ((screeny + (haltonRand(s, 3) - 0.5) * (SAMP_MAXSAMPLES>1 ? 1 : 0) - 0.5 - IMG_H * 0.5) / (double)IMG_H) * -2;
		primaryRay.setDir(dirx, diry, -4);
		primaryRay.dir.normalize();

		prevPixel = outPixel;
		samplePixel = renderPixel(primaryRay, scene);

//		samplePixel.color *= 2.0;
		samplePixel.color.clamp(1.0);
		samplePixel.color.applyGamma(2.2);

		outPixel *= (s - 1.0) / s;

		samplePixel /= s;

		outPixel += samplePixel;

		prevPixel -= outPixel;

		s++;

		if (pow(prevPixel.magnitude(), 2) < SAMP_MAXVARIANCE)
			varianceCombo++;
		else
			varianceCombo = 0;
	}
	screen.setPixel(screenx, screeny, outPixel);
}

int main()
{
	srand(12345);
	
	Scene myScene = readObj(rootdir + "objects/" + objfile + ".obj");

	myScene.skyColor = { 1.2, 1.4, 2.8 };
	myScene.skyColor *= 0.4;
	cout << myScene.numberOfTris(true) << " triangles in scene" << endl;

	clock_t begin = clock();
	myScene.buildBboxHierarchy();
	cout << endl << "Scene build time: " << double(clock() - begin) / CLOCKS_PER_SEC << endl;
	
	Screen myScreen(IMG_W, IMG_H);
	
	cout << endl << "Rendering.. " << endl;

	begin = clock();
	clock_t percent = clock();
	
	//for each row
	for (int y = 0; y < IMG_H; y++ )
	{

		//percentage output
		if (clock() - percent > 1000)
		{
			percent = clock();
			cout << ((double)y * 100.0 / (double)IMG_H) << '%' << endl;
		}

		//for each pixel bunch
#pragma omp parallel for
		for (int x = 0; x < IMG_W; x += NUM_THREADS )
		{
			for (int t = 0; t < NUM_THREADS && t + x < IMG_W; t++)
				renderThread(myScene, myScreen, x + t, y);
		}//end for each pixel
	}//end for each row
	
	cout << endl << "Render time: " << double(clock() - begin) / CLOCKS_PER_SEC << endl;

#ifdef DEBUG_OUTPUTINTERSECTIONS
	myScreen.normalizeValues();
#endif
#ifdef DEBUG_OUTPUTDIST
	myScreen.normalizeValues();
#endif

	writePPM(myScreen, rootdir + objfile + ".ppm", "rgba");

	return 0;
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