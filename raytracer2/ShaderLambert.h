#pragma once
#include "Shader.h"
#include "Ray.h"
#include "ymath.h"

class ShaderLambert :
	public Shader
{
public:

	ShaderLambert()
	{
		Shader::color = { 0.18 };
		Shader::scatterslight = true;
	}

	ShaderLambert(Color& diffuseColorIn)
	{
		Shader::color = diffuseColorIn;
		Shader::scatterslight = true;
	}

	void scatterInRandomDirection(Ray& ray, Vec& n)
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

