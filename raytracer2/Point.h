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
		p.p[0] = xIn;
		p.p[1] = yIn;
		p.p[2] = zIn;
	}

	void setPos(double xIn, double yIn, double zIn)
	{
		p.p[0] = xIn;
		p.p[1] = yIn;
		p.p[2] = zIn;
	}

	void setNormal(double xIn, double yIn, double zIn)
	{
		n.p[0] = xIn;
		n.p[1] = yIn;
		n.p[2] = zIn;
	}

	~Point()
	{
	}
};

