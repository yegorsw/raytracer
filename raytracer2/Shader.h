#pragma once
#include "Ray.h"
#include "Color.h"
class Shader
{
public:

	double alpha = 1.0;
	int samples = 1;
	bool emitslight = false;
	bool scatterslight = false;
	Color color;

	Shader()
	{
	}

	//PDF
	virtual Color getScatterColor(Vec& invec, Vec& outvec, Vec& normal)
	{
		return color;
	}

	//BRDF
	virtual void scatterInRandomDirection(Ray& ray, Ray& cameraRay, Vec& n)
	{
	}

	double hitChance()
	{
		return alpha;
	}

	~Shader()
	{
	}
};

