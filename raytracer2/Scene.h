#pragma once

#include "MtlLib.h"
#include "Mesh.h"

class Scene
{
public:
	vector<Mesh> meshes;
	vector<Mesh> lights;
	
	Scene()
	{
	}

	void addMesh(Mesh mesh)
	{
		meshes.push_back(mesh);
	}

	void addLight(Mesh mesh)
	{
		lights.push_back(mesh);
	}

	int numberOfMeshes()
	{
		return meshes.size();
	}

	void clear()
	{
		meshes.clear();
		lights.clear();
	}

	void printInfo()
	{
		cout << endl << "Number of meshes: " << numberOfMeshes() << endl;
		for (int i = 0; i < numberOfMeshes(); i++)
		{
			cout << meshes[i].numberOfTris() << " tris" << endl;
			meshes[i].boundingBox.printToConsole();
		}

	}

	void buildBboxHierarchy()
	{
		
	}

	~Scene()
	{
	}
};

