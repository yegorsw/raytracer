#pragma once
#include "Vec.h"
#include "globals.h"

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
			vec.p[0] >= minCoord.p[0] && vec.p[0] <= maxCoord.p[0] &&
			vec.p[1] >= minCoord.p[1] && vec.p[1] <= maxCoord.p[1] &&
			vec.p[2] >= minCoord.p[2] && vec.p[2] <= maxCoord.p[2];
	}

	bool intersect(Ray& ray, double& dist)
	{
		if (contains(ray.pos))
		{
			dist = 0.0;
			return true;
		}

		Vec hitpos;
		double planePos = 0;
		double ratio;

		
		//z planes
		if (ray.dir.p[2] != 0)
		{
			if (ray.dir.p[2] > 0)
				planePos = minCoord.p[2];
			else
				planePos = maxCoord.p[2];

			ratio = (planePos - ray.pos.p[2]) / ray.dir.p[2];
			hitpos = ray.pos + (ray.dir * ratio);

			if (hitpos.p[0] >= minCoord.p[0] && hitpos.p[0] <= maxCoord.p[0] && hitpos.p[1] >= minCoord.p[1] && hitpos.p[1] <= maxCoord.p[1])
			{
				if (ratio >= 0.0)
				{
					dist = ratio;
					return true;
				}
			}
		}

		//y planes
		if (ray.dir.p[1] != 0)
		{
			if (ray.dir.p[1] > 0)
				planePos = minCoord.p[1];
			else
				planePos = maxCoord.p[1];

			ratio = (planePos - ray.pos.p[1]) / ray.dir.p[1];
			hitpos = ray.pos + (ray.dir * ratio);

			if (hitpos.p[0] >= minCoord.p[0] && hitpos.p[0] <= maxCoord.p[0] && hitpos.p[2] >= minCoord.p[2] && hitpos.p[2] <= maxCoord.p[2])
			{
				if (ratio >= 0.0)
				{
					dist = ratio;
					return true;
				}
			}
		}

		//x planes
		if (ray.dir.p[0] != 0)
		{
			if (ray.dir.p[0] > 0)
				planePos = minCoord.p[0];
			else
				planePos = maxCoord.p[0];

			ratio = (planePos - ray.pos.p[0]) / ray.dir.p[0];
			hitpos = ray.pos + (ray.dir * ratio);

			if (hitpos.p[1] >= minCoord.p[1] && hitpos.p[1] <= maxCoord.p[1] && hitpos.p[2] >= minCoord.p[2] && hitpos.p[2] <= maxCoord.p[2])
			{
				if (ratio >= 0.0)
				{
					dist = ratio;
					return true;
				}
			}
		}
		
		return false;

	}

	Vec center()
	{
		return (minCoord + maxCoord) * 0.5;
	}

	double volume()
	{
		return (maxCoord.p[0] - minCoord.p[0]) * (maxCoord.p[1] - minCoord.p[1]) * (maxCoord.p[2] - minCoord.p[2]);
	}

	double area()
	{
		Vec box = maxCoord - minCoord;
		return 2.0 * (box.p[0] * box.p[1] + box.p[0] * box.p[2] + box.p[1] * box.p[2]);
	}

	BBox merged(BBox& box)
	{
		return BBox(minCoord.minVec(box.minCoord), maxCoord.maxVec(box.maxCoord));
	}

	int largestAxis()
	{
		double x = maxCoord.p[0] - minCoord.p[0];
		double y = maxCoord.p[1] - minCoord.p[1];
		double z = maxCoord.p[2] - minCoord.p[2];
		double m = max(max(x, y), z);
		if (m == x)
			return 0;
		if (m == y)
			return 1;
		if (m == z)
			return 2;
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

