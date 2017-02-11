#pragma once
#include "Sampler2d.h"
class SamplerHalton :
	public Sampler2d
{
private:
	int basex, basey, offset;

	double result, f;
	int i;
public:
	SamplerHalton()
	{
	}

	SamplerHalton(int offsetIn, int basexIn, int baseyIn)
	{
		offset = offsetIn;
		basex = basexIn;
		basey = baseyIn;
	}

	void getNextSample(double& xout, double& yout)
	{
		result = 0;
		f = 1;
		i = offset;
		while (i > 0)
		{
			f /= double(basex);
			result += f * (i % basex);
			i /= basex;
		}

		xout = result;

		result = 0;
		f = 1;
		i = offset;
		while (i > 0)
		{
			f /= double(basey);
			result += f * (i % basey);
			i /= basey;
		}

		yout = result;

		offset++;
	}

	~SamplerHalton()
	{
	}
};

