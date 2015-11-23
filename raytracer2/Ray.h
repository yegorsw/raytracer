#pragma once
#include "Vec.h"
class Ray
{
public:
	Vec p;
	Vec dir;

	Vec hitPoint;
	

	Ray()
	{
	}

	Ray(Vec pIn, Vec dirIn)
	{
		p = pIn;
		dir = dirIn;
	}

	void setDir(double x, double y, double z)
	{
		dir.x = x;
		dir.y = y;
		dir.z = z;
	}

	void setPos(double x, double y, double z)
	{
		p.x = x;
		p.y = y;
		p.z = z;
	}

	void setPos(Vec v)
	{
		p.x = v.x;
		p.y = v.y;
		p.z = v.z;
	}
	
	~Ray()
	{
	}
};

