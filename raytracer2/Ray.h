#pragma once
#include "Vec.h"
class Ray
{
public:
	Vec pos;
	Vec dir;

	Ray()
	{
	}

	Ray(Vec posIn, Vec dirIn)
	{
		pos = posIn;
		dir = dirIn;
	}

	void setDir(double x, double y, double z)
	{
		dir.p[0] = x;
		dir.p[1] = y;
		dir.p[2] = z;
	}

	void setPos(double x, double y, double z)
	{
		pos.p[0] = x;
		pos.p[1] = y;
		pos.p[2] = z;
	}

	void setPos(Vec v)
	{
		pos.p[0] = v.p[0];
		pos.p[1] = v.p[1];
		pos.p[2] = v.p[2];
	}
	
	~Ray()
	{
	}
};

