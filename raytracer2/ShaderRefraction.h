#pragma once
#include "Shader.h"
#include "Ray.h"
#include "ymath.h"
#include "ShaderFunctions.h"

class ShaderRefraction:
	public Shader
{
public:

	double ior = 1.5;

	ShaderRefraction(Color col)
	{
		color = col;
		scatterslight = true;
	}

	Color getScatterColor(SampleInfo& SI)
	{
		//return Color(1);
		double fr = 1.0 - fresnel(SI.inRay->dir.dot(*SI.normal), 1.0, ior);
		return color * fr;
	}

	void scatterInRandomDirection(SampleInfo& SI)
	{
		//SI.outRay->dir = SI.inRay->dir;
		
		//if normal faces opposite direction of penetration ray
		
		if (SI.inRay->dir.dot(*SI.normal) < 0.0)
			SI.outRay->dir = refract(SI.inRay->dir, -*SI.normal, 1.0, 1.6);
		else
			SI.outRay->dir = refract(SI.inRay->dir, *SI.normal, 1.6, 1.0);
			
	}

	~ShaderRefraction()
	{
	}
};