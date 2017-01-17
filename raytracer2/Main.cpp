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

		//light scatter loop
		if (depth < SCN_MAXDEPTH && closestTri->mtl->scatterslight)
		{			
			bool traceNextBounce = false;
			const int numsamples = depth == 0 ? SHD_MAXSAMPLES : 1;
			
			Shader* shader;

			for (int i = 0; i < numsamples; i++)
			{
				shader = closestTri->mtl->selectShaderStochastically();

				if (shader)
				{
					Ray indirectRay;
					Pixel nextBounce;

					indirectRay.setPos(ray.pos + (ray.dir * shortestDist) + (closestTri->n * SCN_RAYBIAS));
					shader->scatterInRandomDirection(indirectRay, ray, closestTri->n);

					nextBounce = renderPixel(indirectRay, scene, depth + 1);
					nextBounce.color *= shader->getColor();
					nextBounce.color *= indirectRay.dir.dot(closestTri->n);

					outPixel.color += nextBounce.color;
				}
			}
			outPixel /= numsamples;
			outPixel.a = 1.0;
			return outPixel;
		}//end light scatter loop

		 //light emission loop
		if (closestTri->mtl->emitslight)
		{
			for (vector<Shader*>::iterator s = shaderstack.begin(); s != shaderstack.end(); s++)
			{
				if ((**s).emitslight)
				{
					outPixel.color += ((**s).getColor() * (**s).hitChance());
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
	double s = 1.0;
	Pixel samplePixel, outPixel, prevPixel;

	Ray primaryRay;

	int varianceCombo = 0;

	double dirx, diry;

	while (varianceCombo < SAMP_MINSAMPLES && s <= SAMP_MAXSAMPLES)
	{
		dirx = ((screenx + randfneg() * 0.5 * (SAMP_MAXSAMPLES>1 ? 1 : 0) - 0.5 - IMG_W * 0.5) / (double)IMG_H) * 2;
		diry = ((screeny + randfneg() * 0.5 * (SAMP_MAXSAMPLES>1 ? 1 : 0) - 0.5 - IMG_H * 0.5) / (double)IMG_H) * -2;
		primaryRay.setDir(dirx, diry, -2);
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