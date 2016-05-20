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
	
	~Scene()
	{
	}
};

