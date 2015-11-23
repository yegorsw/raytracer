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
const int IMG_W = 400;
const int IMG_H = 400;

const double SCN_MAXDIST = 5000;
const double SCN_MAXRAYDIST = 5000;
const double SCN_RAYBIAS = 0.1;
const int SCN_MAXDEPTH = 1;

const int SCN_SHADERSAMPLES = 1;
const int SAMP_MINSAMPLES = 4;
const int SAMP_MAXSAMPLES = 512;
const double SAMP_MINVARIANCE = 0.1;

const bool SCN_JITTER = true;
////END OF SCENE SETUP////

const double invRandMax2 = (2.0 / (double)RAND_MAX);
const double invRandMax = (1.0 / (double)RAND_MAX);
const double invShaderSamples = 1.0 / (double)SCN_SHADERSAMPLES;

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
	double intersectDist = 0;
	double shortestDist = 0;

	if (depth > 0)
		shortestDist = SCN_MAXDIST;
	else
		shortestDist = SCN_MAXRAYDIST;

	Tri* closestTri = &(scene.meshes[0].triList[0]);

	for (int obj = 0; obj < scene.numberOfMeshes(); obj++)
	{
		Mesh& mesh = scene.meshes[obj];
		if (!mesh.hasBbox || mesh.boundingBox.intersect(ray))
		{
			for (int tri = 0; tri < mesh.numberOfTris(); tri++)
			{
				intersectDist = mesh.triList[tri].intersect(ray);
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

			for (int i = 0; i < SCN_SHADERSAMPLES; i++)
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
			outPixel *= invShaderSamples;
			outPixel.a = 1.0;
			return outPixel;//test
		}
	}
	else//if depth == SCN_MAXDEPTH (last bounce)
	{
		if (shortestDist < SCN_MAXDIST)
			return Pixel(1, 1, 1, 1);
	}

	return Pixel(0, 0, 0, 0);
}

int main()
{
	srand(12345);

	string objfile = "lamp";
	Scene myScene = readObj("D:/Users/Yegor/Desktop/raytracer/objects/"+objfile+".obj");
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
			for (int s = 0; s < SAMP_MAXSAMPLES; s++)
			{
				sx = ((x + randfneg()*0.5 - IMG_W * 0.5) / (float)IMG_H) * 2;
				sy = ((y + randfneg()*0.5 - IMG_H * 0.5) / (float)IMG_H) * -2;

				ss = (double)s + 1.0;

				primaryRay.setDir(sx* 0.6, sy*0.6, -1.0);
				primaryRay.dir.normalize();

				variance = currentPixel;

				renderedPixel = renderPixel(primaryRay, myScene);
				renderedPixel *= 1.0 / ss;
				currentPixel *= (ss - 1.0) / ss;
				currentPixel += renderedPixel;

				variance -= currentPixel;

				if (s >= SAMP_MINSAMPLES)
				{
					if (variance.magnitude() < SAMP_MINVARIANCE)
					{
						varianceCombo++;
						if (varianceCombo > SAMP_MINSAMPLES)
							break;
					}
					else
					{
						varianceCombo = 0;
					}
				}
			}

#ifdef DEBUG_OUTPUTSAMPLES
			double samplesUsed = ss / (double)SAMP_MAXSAMPLES;
			currentPixel.setColor(samplesUsed, samplesUsed, samplesUsed);
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
	cout << endl << "Render time: " << double(end - begin) / CLOCKS_PER_SEC << endl;

	myScreen.invertRGB();
	//myScreen.constantAlpha();
	writePPM(myScreen, "D:/Users/Yegor/Desktop/raytracer/"+objfile+".ppm");
	//myScene.printInfo();
	//int pause; cin >> pause;
}


/*
double l = length(sx, sy);
sx = (sx * (1 - sqrt(1 - l * l))) * curvature + sx * (1.0 - curvature);
sy = (sy * (1 - sqrt(1 - l * l))) * curvature + sy * (1.0 - curvature);
*/