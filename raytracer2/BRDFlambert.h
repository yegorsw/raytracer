#pragma once
#include "BRDF.h"
#include "Vec.h"
class BRDFlambert :
	public BRDF
{
public:

	BRDFlambert()
	{
	}

	void scatterInRandomDirection(Vec& v)
	{

	}

	~BRDFlambert()
	{
	}
};

