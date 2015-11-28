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

	double intersect(Ray& ray)
	{
		Vec hitpos;		
		double planePos = 0;
		double ratio;

		//z planes
		if (ray.dir.z != 0)
		{
			if (ray.dir.z > 0)
				planePos = minCoord.z;
			else
				planePos = maxCoord.z;

			ratio = (planePos - ray.p.z) / ray.dir.z;
			hitpos = ray.p + (ray.dir * ratio);

			if (hitpos.x >= minCoord.x && hitpos.x <= maxCoord.x && hitpos.y >= minCoord.y && hitpos.y <= maxCoord.y)
				return ratio;
		}

		//y planes
		if (ray.dir.y != 0)
		{
			if (ray.dir.y > 0)
				planePos = minCoord.y;
			else
				planePos = maxCoord.y;

			ratio = (planePos - ray.p.y) / ray.dir.y;
			hitpos = ray.p + (ray.dir * ratio);

			if (hitpos.x >= minCoord.x && hitpos.x <= maxCoord.x && hitpos.z >= minCoord.z && hitpos.z <= maxCoord.z)
				return ratio;
		}

		//z planes
		if (ray.dir.x != 0)
		{
			if (ray.dir.x > 0)
				planePos = minCoord.x;
			else
				planePos = maxCoord.x;

			ratio = (planePos - ray.p.x) / ray.dir.x;
			hitpos = ray.p + (ray.dir * ratio);

			if (hitpos.y >= minCoord.y && hitpos.y <= maxCoord.y && hitpos.z >= minCoord.z && hitpos.z <= maxCoord.z)
				return ratio;
		}
		
		return 0.0;

	}

	Vec center()
	{
		return (minCoord + maxCoord) * 0.5;
	}

	double volume()
	{
		return (maxCoord.x - minCoord.x) * (maxCoord.y - minCoord.y) * (maxCoord.z - minCoord.z);
	}

	BBox merged(BBox& box)
	{
		return BBox(minCoord.minVec(box.minCoord), maxCoord.maxVec(box.maxCoord));
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

