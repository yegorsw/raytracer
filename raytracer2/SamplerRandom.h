#pragma once
#include "Sampler2d.h"
#include "ymath.h"
class SamplerRandom :
	public Sampler2d
{
public:

	SamplerRandom()
	{
	}

	void getNextSample(double& xout, double& yout)
	{
		xout = randf();
		yout = randf();
	}

	~SamplerRandom()
	{
	}
};

