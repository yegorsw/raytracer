#pragma once
#include "vec.h"
class Point
{
private:
	Vec n;
	Vec p;

public:

	Point()
	{
	}

	Point(double xIn, double yIn, double zIn)
	{
		p.x = xIn;
		p.y = yIn;
		p.z = zIn;
	}

	void setPos(double xIn, double yIn, double zIn)
	{
		p.x = xIn;
		p.y = yIn;
		p.z = zIn;
	}

	void setNormal(double xIn, double yIn, double zIn)
	{
		n.x = xIn;
		n.y = yIn;
		n.z = zIn;
	}

	~Point()
	{
	}
};

