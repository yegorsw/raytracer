#pragma once

#include "MtlLib.h"
#include "GeoContainer.h"
#include "globals.h"

class Scene: public GeoContainer
{
private:
	vector<Tri*> lightList;

public:
	//	vector<Mesh> meshes;
	//	vector<Mesh> lights;
	//	GeoContainer masterContainer; test
	MtlLib* mlib;
	Color skyColor;

	Scene()
	{
	}


	void buildLightList()
	{

	}

	bool intersect(Ray& ray, Tri*& closestTri, double& shortestDist, int& numBoxIntersections, int depth = 0)
	{
		double _dummy;

		if (boundingBox.intersect(ray, _dummy))
		{
			numBoxIntersections++; 
			return GeoContainer::intersect(ray, closestTri, shortestDist, numBoxIntersections, depth);
		}
		else
			return false;
	}

	~Scene()
	{
	}
};

