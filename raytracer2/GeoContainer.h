#pragma once

#include "Mesh.h"

class GeoContainer
{
public:
	BBox boundingBox;
	vector<Mesh> meshes;
	vector<GeoContainer> containers;
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

	int numberOfContainers()
	{
		return containers.size();
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
		for (int i = 0; i < numberOfContainers(); i++)
		{
			boundingBox = boundingBox.merged(containers[i].boundingBox);
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
				dist = fabs((*inputMesh).boundingBox.merged((*m).boundingBox).volume());
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

	void buildBboxHierarchy(int depth=0)
	{
		int mpc = 2;//meshes per lowest level container
		if (numberOfMeshes() > mpc)
		{
			//meshes for each subcontainer
			int msc = (numberOfMeshes() / mpc) + (numberOfMeshes() % mpc > 0 ? 1 : 0);//hacky way to do ceil() on 2 ints

			//cout << "Building Hierarchy Depth " << depth << ": " << mtc << "meshes in each subcontainer" << endl;

			GeoContainer newContainer;
			int i = 0;
			vector<Mesh>::iterator currentMesh = meshes.begin() + (rand() % (numberOfMeshes() - 1));
			while (numberOfMeshes() > mpc)
			{
				if (i < msc)
				{
					vector<Mesh>::iterator bestPartner = findBestPartner(currentMesh);
					moveMeshToGroup(bestPartner, newContainer);
					i++;
				}

				if (i == msc || numberOfMeshes() == 0)
				{
					//cout << "  adding subcontainer" << endl;
					moveMeshToGroup(currentMesh, newContainer);
					
					newContainer.buildBboxHierarchy(depth + 1);
					newContainer.parent = this;
					containers.push_back(newContainer);

					GeoContainer newContainer;
					i = 0;
					if (numberOfMeshes() > 0)
						currentMesh = meshes.begin() + (rand() % (numberOfMeshes() - 1));
					
				}

			}
			
		}
		calculateBbox();
	}

	bool intersect(Ray& ray, Tri*& closestTri, double& shortestDist, int depth = 0)
	{
		bool contains = boundingBox.contains(ray.p);
		double intersectDist = 0.0;

		if (!contains)
		{
			intersectDist = boundingBox.intersect(ray);
		}

		if (contains || (intersectDist > 0.0 && intersectDist < shortestDist))
		{
			for (int i = 0; i < numberOfContainers(); i++)
			{
				containers[i].intersect(ray, closestTri, shortestDist);
			}
			for (int i = 0; i < numberOfMeshes(); i++)
			{
				meshes[i].intersect(ray, closestTri, shortestDist);
			}
		}
		
		return (shortestDist > 0.0 && shortestDist < 4999.9999);
	}

	int numberOfTris()
	{
		int tris = 0;
		for (int i = 0; i < numberOfMeshes(); i++)
		{
			tris += meshes[i].numberOfTris();
		}
		for (int i = 0; i < numberOfContainers(); i++)
		{
			tris += containers[i].numberOfTris();
		}

		return tris;
	}

	void printToConsole(int depth = 0)
	{
		cout << endl << string(depth * 3, ' ') << "Number of meshes: " << numberOfMeshes() << endl;
		cout << endl << string(depth * 3, ' ');
		boundingBox.minCoord.printToConsole();
		cout << endl << string(depth * 3, ' ');
		boundingBox.maxCoord.printToConsole();

		cout << endl;
		for (int i = 0; i < numberOfContainers(); i++)
		{
			containers[i].printToConsole(depth + 1);
		}

		if (depth == 0)
			cout << numberOfTris() << " total tris" << endl;

	}



	~GeoContainer()
	{
	}
};

/*
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
*/