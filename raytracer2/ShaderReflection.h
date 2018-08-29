#pragma once
#include "Shader.h"
#include "Ray.h"
#include "ymath.h"
#include "ShaderFunctions.h"

class ShaderReflection :
	public Shader
{
public:

	double ior = 1.6;

	ShaderReflection(Color col)
	{
		color = col;
		scatterslight = true;
	}

	Color getScatterColor(SampleInfo& SI)
	{
		double d = SI.inRay->dir.dot(*SI.normal);
		if (d > 0)
			return Color(0);
		double fr = fresnel(d, 1.0, ior);
		return color * fr;
	}

	void scatterInRandomDirection(SampleInfo& SI)
	{
		SI.outRay->dir = reflect(SI.inRay->dir, *SI.normal);
	}

	~ShaderReflection()
	{
	}
};

//		Vec halfvector = invec + outvec;
//		halfvector.normalize();