#pragma once
#include "Vec.h"

class BBox
{
public:

	Vec minCoord, maxCoord;

	BBox()
	{
	}

	BBox(Vec minCoordIn, Vec maxCoordIn)
	{
		minCoord = minCoordIn;
		maxCoord = maxCoordIn;
	}

	bool contains(Vec &vec)
	{
		return
			vec.x >= minCoord.x && vec.x <= maxCoord.x &&
			vec.y >= minCoord.y && vec.y <= maxCoord.y &&
			vec.z >= minCoord.z && vec.z <= maxCoord.z;
	}

	bool intersect(Ray& ray)
	{
		if (contains(ray.p))
			return true;

		Vec hitpos;		
		double planePos = 0;

		//z planes
		if (ray.dir.z != 0)
		{
			if (ray.dir.z > 0)
				planePos = minCoord.z;
			else
				planePos = maxCoord.z;

			double ratio = (planePos - ray.p.z) / ray.dir.z;
			hitpos = ray.p + (ray.dir * ratio);

			if (hitpos.x >= minCoord.x && hitpos.x <= maxCoord.x && hitpos.y >= minCoord.y && hitpos.y <= maxCoord.y)
				return true;
		}

		//y planes
		if (ray.dir.y != 0)
		{
			if (ray.dir.y > 0)
				planePos = minCoord.y;
			else
				planePos = maxCoord.y;

			double ratio = (planePos - ray.p.y) / ray.dir.y;
			hitpos = ray.p + (ray.dir * ratio);

			if (hitpos.x >= minCoord.x && hitpos.x <= maxCoord.x && hitpos.z >= minCoord.z && hitpos.z <= maxCoord.z)
				return true;
		}

		//z planes
		if (ray.dir.x != 0)
		{
			if (ray.dir.x > 0)
				planePos = minCoord.x;
			else
				planePos = maxCoord.x;

			double ratio = (planePos - ray.p.x) / ray.dir.x;
			hitpos = ray.p + (ray.dir * ratio);

			if (hitpos.y >= minCoord.y && hitpos.y <= maxCoord.y && hitpos.z >= minCoord.z && hitpos.z <= maxCoord.z)
				return true;
		}
		
		return false;

	}

	Vec center()
	{
		return (minCoord + maxCoord) * 0.5;
	}

	void printToConsole()
	{
		minCoord.printToConsole();
		maxCoord.printToConsole();
	}

	void clear()
	{
		minCoord = Vec();
		maxCoord = Vec();
	}

	~BBox()
	{
	}
};

