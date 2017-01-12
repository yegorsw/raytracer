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

using namespace std;

Pixel renderPixel(Ray& ray, Scene& scene, int depth = 0)
{
	double intersectDist;
	double shortestDist = SCN_MAXDIST;
	
	Tri* closestTri;
	bool intersected;

	intersected = scene.intersect(ray, closestTri, shortestDist);
	
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
	Pixel samplePixel, outPixel, prevPixel;

	Ray primaryRay;

	int varianceCombo = 0;

	double dirx, diry;

	while (varianceCombo < SAMP_MINSAMPLES && s <= SAMP_MAXSAMPLES)
	{
		
		dirx = ((screenx + randfneg() * 0.5 * (SAMP_MAXSAMPLES>1 ? 1 : 0) - 0.5 - IMG_W * 0.5) / (double)IMG_H) * 2;
		diry = ((screeny + randfneg() * 0.5 * (SAMP_MAXSAMPLES>1 ? 1 : 0) - 0.5 - IMG_H * 0.5) / (double)IMG_H) * -2;
		primaryRay.setDir(dirx, diry, -4);
		primaryRay.dir.normalize();

		prevPixel = outPixel;
		samplePixel = renderPixel(primaryRay, scene);

		samplePixel.color *= 2.0;
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

	string objfile = "angel";
	string rootdir = "C:/Users/Yegor-s/Desktop/raytracer/";
	/*
	Scene myScene;
	Tri* t = new Tri(Vec(1, 1, -15), Vec(-1, 0.5, -15), Vec(-1.2, 0, -15));
	t->precompute();
	myScene.child1 = new GeoContainer;
	myScene.child1->child2 = new GeoContainer;
	myScene.child1->child2->child1 = new GeoContainer;
	myScene.child1->child2->child2 = new GeoContainer;
	myScene.child1->child2->child1->addTri(t);
	myScene.child1->child2->child2->addTri(t);
	myScene.child1->child2->child1->generateBbox();
	myScene.boundingBox = myScene.child1->child2->child1->boundingBox;
	*/

	Scene myScene = readObj(rootdir + "objects/" + objfile + ".obj");


//	myScene.skyColor.Ka = { 1.2, 1.4, 2.8 };
//	myScene.skyColor.Ka *= 0.5;
	cout << myScene.numberOfTris(true) << " triangles in scene" << endl;
	myScene.buildBboxHierarchy();
//	myScene.split(0.5, 2);

	cout << myScene.numberOfTris(true) << " triangles in scene" << endl;

//	myScene.findEmptyChildren();

//	myScene.printToConsole();

	Screen myScreen(IMG_W, IMG_H);
	
	cout << endl << "Rendering.. " << endl;

	clock_t begin = clock();
	clock_t percent = clock();

	//thread threads[NUM_THREADS];

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
		for (int x = 0; x < IMG_W; x += NUM_THREADS )
		{
			for (int t = 0; t < NUM_THREADS && t + x < IMG_W; t++)
				renderThread(myScene, myScreen, x + t, y);
			//	threads[t] = thread(renderThread, myScene, myScreen, x + t, y);

			//for (int t = 0; t < NUM_THREADS && t + x < IMG_W; t++)
			//	threads[t].join();

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
	//myScreen.invertRGB();
	//myScreen.constantAlpha();

#ifdef DEBUG_OUTPUTINTERSECTIONS
	myScreen.normalizeValues();
#endif
#ifdef DEBUG_OUTPUTDIST
	myScreen.normalizeValues();
#endif

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
