#pragma once
#include "Color.h"
#include "SampleInfo.h"
class Shader
{
public:

	double alpha = 1.0;
	int samples = 1;
	bool emitslight = false;
	bool scatterslight = false;
	Color color;

	Shader()
	{
	}

	//PDF
	virtual Color getScatterColor(SampleInfo& SI)
	{
		return color;
	}

	//BRDF
	virtual void scatterInRandomDirection(SampleInfo& SI)
	{
	}

	virtual int getNumSamples()
	{
		return samples;
	}

	double hitChance()
	{
		return alpha;
	}

	virtual ~Shader()
	{
	}
};

