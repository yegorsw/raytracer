#pragma once

#include "MtlLib.h"
#include "GeoContainer.h"

class Scene: public GeoContainer
{
private:
	vector<Tri*> lightList;

public:
	//	vector<Mesh> meshes;
	//	vector<Mesh> lights;
	//	GeoContainer masterContainer;
	MtlLib* mlib;

	Scene()
	{
	}


	void buildLightList()
	{

	}

	bool intersect(Ray& ray, Tri*& closestTri, double& shortestDist, int depth = 0)
	{
		double _dummy;
		if (boundingBox.intersect(ray, _dummy))
			return GeoContainer::intersect(ray, closestTri, shortestDist, depth = 0);
		else
			return false;
	}

	~Scene()
	{
	}
};

