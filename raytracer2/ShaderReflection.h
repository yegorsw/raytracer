#pragma once
#include "Shader.h"
#include "Ray.h"
#include "ymath.h"

class ShaderReflection :
	public Shader
{
public:

	ShaderReflection()
	{
		Shader::color = { 0.5 };
		Shader::scatterslight = true;
	}

	ShaderReflection(Color& reflColorIn)
	{
		Shader::color = reflColorIn;
		Shader::alpha = 0.5;
		Shader::scatterslight = true;
	}

	void scatterInRandomDirection(Ray& ray, Ray& cameraRay, Vec& n)
	{
		ray.dir = (n * (-cameraRay.dir).dot(n) * 2.0) + cameraRay.dir;
	}

	~ShaderReflection()
	{
	}
};
