#pragma once
#include "Shader.h"
#include "Ray.h"
#include "ymath.h"
#include "ShaderFunctions.h"

class ShaderReflection :
	public Shader
{
public:

	double ior = 1.5;

	ShaderReflection(Color col)
	{
		color = col;
		scatterslight = true;
	}

	Color getScatterColor(Vec& invec, Vec& outvec, Vec& normal)
	{
//		Vec halfvector = invec + outvec;
//		halfvector.normalize();
		double fr = fresnel(invec.dot(normal), 1.0, ior);
		return color * fr * alpha;
	}

	void scatterInRandomDirection(Ray& ray, Ray& cameraRay, Vec& n)
	{
		ray.dir = (n * (-cameraRay.dir).dot(n) * 2.0) + cameraRay.dir;
	}

	~ShaderReflection()
	{
	}
};

