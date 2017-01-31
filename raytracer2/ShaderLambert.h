#pragma once
#include "Shader.h"
#include "Ray.h"
#include "ymath.h"

class ShaderLambert :
	public Shader
{
public:

	ShaderLambert(Color col)
	{
		Shader::color = col;
		Shader::scatterslight = true;
		Shader::samples = 8;
	}

	void scatterInRandomDirection(Ray& ray, Ray& cameraRay, Vec& n)
	{
		do ray.setDir(randfneg(), randfneg(), randfneg());
		while (ray.dir.dot(ray.dir) > 1.0);

		ray.dir.normalize();

		if (ray.dir.dot(n) < 0.0)
			ray.dir = -ray.dir;
	}

	~ShaderLambert()
	{
	}
};

