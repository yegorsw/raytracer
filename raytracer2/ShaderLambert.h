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
		Shader::samples = 8;
	}

	void scatterInRandomDirection(SampleInfo& SI)
	{
		double rand1, rand2;
		SI.sampleGenerator->getNextSample(rand1, rand2);

		double r = rand1;
		double phi = rand2 * 2 * PI;

		//cosine weighting
		r = cos(r * 0.5 * PI);

		double x = r * sin(phi);
		double y = r * cos(phi);
		double z = sqrt(1.0 - r * r);

		SI.outRay->setDir(x, y, z);

		SI.outRay->dir.transformTo(*SI.xaxis, *SI.yaxis, *SI.normal);
	}

	~ShaderLambert()
	{
	}
};

