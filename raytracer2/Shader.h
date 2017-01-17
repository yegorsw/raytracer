#pragma once
#include "Ray.h"
#include "Color.h"
class Shader
{
public:

	double alpha = 1.0;
	bool emitslight = false;
	bool scatterslight = false;
	Color color;

	Shader()
	{
	}

	Color getColor() { return color; }

	virtual void scatterInRandomDirection(Ray& ray, Vec& n) {}

	double hitChance()
	{
		return alpha;
	}

	~Shader()
	{
	}
};

