#pragma once
#include "Ray.h"
#include "Color.h"
#include "Sampler2d.h"

//container for shader access to various data
class SampleInfo
{
public:

	//3d scene variables
	Ray* inRay;
	Ray* outRay;
	Vec* normal;
	Vec* geoNormal;

	//rng/dmc sample generator
	Sampler2d* sampleGenerator;

	//orthonormal basis
	Vec* xaxis;
	Vec* yaxis;

	SampleInfo()
	{
	}

	~SampleInfo()
	{
	}
};

