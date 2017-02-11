#pragma once
class Sampler2d
{
public:

	Sampler2d()
	{
	}

	virtual void getNextSample(double& xout, double& yout)
	{
	}

	virtual ~Sampler2d()
	{
	}
};

