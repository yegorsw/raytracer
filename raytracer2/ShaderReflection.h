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

	Color getScatterColor(SampleInfo& SI)
	{
		double fr = fresnel(SI.inRay->dir.dot(*SI.normal), 1.0, ior);
		return color * fr * alpha;
	}

	void scatterInRandomDirection(SampleInfo& SI)
	{
		SI.outRay->dir = (*SI.normal * (-SI.inRay->dir).dot(*SI.normal) * 2.0) + SI.inRay->dir;
	}

	~ShaderReflection()
	{
	}
};

//		Vec halfvector = invec + outvec;
//		halfvector.normalize();