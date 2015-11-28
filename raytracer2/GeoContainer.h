#pragma once

#include "Mesh.h"

class GeoContainer
{
public:
	BBox boundingBox;
	vector<Mesh> meshes;
	vector<GeoContainer> children;
	GeoContainer* parent;


	GeoContainer()
	{
	}

	void addMesh(Mesh mesh)
	{
		meshes.push_back(mesh);
	}

	int numberOfMeshes()
	{
		return meshes.size();
	}

	void clear()
	{
		meshes.clear();
	}

	void calculateBbox()
	{
		boundingBox = meshes[0].boundingBox;
		for (int i = 0; i < numberOfMeshes(); i++)
		{
			boundingBox = boundingBox.merged(meshes[i].boundingBox);
		}
	}

	vector<Mesh>::iterator findBestPartner(vector<Mesh>::iterator inputMesh)
	{
		int smallestDist = -1;
		int dist;
		vector<Mesh>::iterator bestPartner;
		for (vector<Mesh>::iterator m = meshes.begin(); m != meshes.end(); m++)
		{
			if (m != inputMesh)
			{
				dist = ((*inputMesh).boundingBox.center() - (*m).boundingBox.center()).length();
				if (smallestDist < 0 || dist < smallestDist)
				{
					smallestDist = dist;
					bestPartner = m;
				}
			}
		}
		return bestPartner;
	}

	void moveMeshToGroup(vector<Mesh>::iterator m, GeoContainer& container)
	{
		container.addMesh(*m);
		meshes.erase(m);
	}

	void buildBboxHierarchy()
	{
		int mpc = 5;//meshes per container
		while (numberOfMeshes() > mpc)
		{
			GeoContainer newContainer;
			newContainer.parent = this;

			//get random mesh in scene
			vector<Mesh>::iterator currentMesh = meshes.begin() + (rand() % meshes.size());
			vector<Mesh>::iterator bestPartner;
			for (int i = 0; i < mpc; i++)
			{
				bestPartner = findBestPartner(currentMesh);
				moveMeshToGroup(bestPartner, newContainer);
			}

			newContainer.calculateBbox();
			children.push_back(newContainer);
		}
			
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

	~GeoContainer()
	{
	}
};

