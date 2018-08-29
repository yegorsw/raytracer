#pragma once
#include "Shader.h"
#include "ymath.h"

class ShaderLambert :
	public Shader
{
public:

	ShaderLambert(Color col)
	{
		Shader::color = col;
		Shader::scatterslight = true;
		Shader::samples = 64;
	}
	
	void scatterInRandomDirection(SampleInfo& SI)
	{
		double rand1, rand2;
		SI.sampleGenerator->getNextSample(rand1, rand2);

		double r = sqrt(rand1);
		double phi = rand2 * 2 * PI;

		double x = r * sin(phi);
		double y = r * cos(phi);
		double z = sqrt(1.0 - rand1);

		SI.outRay->setDir(x, y, z);

		SI.outRay->dir.transformTo(*SI.xaxis, *SI.yaxis, *SI.normal);
	}

	Color getScatterColor(SampleInfo &SI)
	{
		return color / (PI * 0.5);
	}
	/*
	void scatterInRandomDirection(SampleInfo& SI)
	{
		do { 
			SI.outRay->setDir(randfneg(), randfneg(), randfneg()); 
		} while	(SI.outRay->dir.dot(SI.outRay->dir) > 1.0);

		SI.outRay->dir.normalize();
	}

	Color getScatterColor(SampleInfo &SI)
	{
		return Color(SI.outRay->dir.dot(*SI.normal) * 0.1);
	}
	*/
	~ShaderLambert()
	{
	}
};

