#pragma once

#include "Mesh.h"
#include "globals.h"

class GeoContainer
{
public:
	BBox boundingBox;
	vector<Mesh*> meshes;
	vector<GeoContainer> containers;
	GeoContainer* parent;


	GeoContainer()
	{
	}

	void addMesh(Mesh* mesh)
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
		if (numberOfMeshes() > 0)
			boundingBox = meshes[0]->boundingBox;
		else if (numberOfContainers() > 0)
			boundingBox = containers[0].boundingBox;

		for (int i = 0; i < numberOfMeshes(); i++)
		{
			boundingBox = boundingBox.merged(meshes[i]->boundingBox);
		}
		for (int i = 0; i < numberOfContainers(); i++)
		{
			boundingBox = boundingBox.merged(containers[i].boundingBox);
		}
	}

	/*
	void mergeMeshes()
	{
		Mesh masterMesh;
		for (vector<Mesh*>::iterator m = meshes.begin(); m != meshes.end(); m++)
		{
			move((*m)->triList.begin(), (*m).triList.end(), back_inserter(masterMesh.triList));
		}
		clear();
		masterMesh.generateBbox();
		meshes.push_back(masterMesh);
	}*/

	void moveMeshToGroup(vector<Mesh*>::iterator& m, GeoContainer& container)
	{
		move(m, m+1, back_inserter(container.meshes));
		
		//this automatically increments the mesh pointer to the next mesh
		m = meshes.erase(m);
	}

	void buildBboxHierarchy(int depth=0)
	{
		calculateBbox();

		if (numberOfMeshes() > 2 && depth < 4)
		{

			if (depth < 30)
				cout << string(depth*2, ' ') << "Sorting Depth " << depth << endl;

			GeoContainer* child1 = new GeoContainer;
			GeoContainer* child2 = new GeoContainer;

			for (vector<Mesh*>::iterator m = meshes.begin(); m != meshes.end();)
			{
				if (boundingBox.largestAxis() == 0)
				{
					if ((**m).boundingBox.center().x < boundingBox.center().x)
						moveMeshToGroup(m, *child1);
					else
						moveMeshToGroup(m, *child2);
				}
				else if (boundingBox.largestAxis() == 1)
				{
					if ((**m).boundingBox.center().y < boundingBox.center().y)
						moveMeshToGroup(m, *child1);
					else
						moveMeshToGroup(m, *child2);
				}
				else if (boundingBox.largestAxis() == 2)
				{
					if ((**m).boundingBox.center().z < boundingBox.center().z)
						moveMeshToGroup(m, *child1);
					else
						moveMeshToGroup(m, *child2);
				}
				
			}

			(*child1).buildBboxHierarchy(depth + 1);
			(*child2).buildBboxHierarchy(depth + 1);

			containers.push_back(move(*child1));
			containers.push_back(move(*child2));


			(*child1).parent = this;
			(*child2).parent = this;

		}//end if num meshes > 2

		/*
		for (vector<Mesh>::iterator m = meshes.begin(); m != meshes.end(); m++)
		{
			(*m).buildBboxHierarchy();
		}*/

	}

	void findEmptyChildren()
	{
		for (vector<GeoContainer>::iterator c = containers.begin(); c != containers.end(); c++)
		{
			c->findEmptyChildren();
		}
		if (numberOfContainers() + numberOfMeshes() < 2)
			cout << "DAMN!!!!\n";
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
			for (int i = 0; i < numberOfMeshes(); i++)
			{
				meshes[i]->intersect(ray, closestTri, shortestDist);
			}

			for (int i = 0; i < numberOfContainers(); i++)
			{
				containers[i].intersect(ray, closestTri, shortestDist);
			}

		}


	return (shortestDist > 0.0 && shortestDist < 4999.9999);
	}

	/*bool intersect(Ray& ray, Tri*& closestTri, double& shortestDist, int depth = 0)
	{
		bool contains = boundingBox.contains(ray.p);
		double intersectDist = 0.0;
		
		if (!contains)
		{
			intersectDist = boundingBox.intersect(ray);
		}

		if (contains || (intersectDist > 0.0 && intersectDist < shortestDist))
		{
			for (int i = 0; i < numberOfMeshes(); i++)
			{
				meshes[i]->intersect(ray, closestTri, shortestDist);
			}

			for (int i = 0; i < numberOfContainers(); i++)
			{
				containers[i].intersect(ray, closestTri, shortestDist);
			}

		}
		
		
		return (shortestDist > 0.0 && shortestDist < 4999.9999);
	}*/

	int numberOfTris()
	{
		int tris = 0;
		for (int i = 0; i < numberOfMeshes(); i++)
		{
			tris += meshes[i]->numberOfTris();
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
vector<Mesh>::iterator findBestPartner(GeoContainer& container)
{
int smallestDist = -1;
int dist;
vector<Mesh>::iterator bestPartner;
for (vector<Mesh>::iterator m = meshes.begin(); m != meshes.end(); m++)
{
dist = (container.boundingBox.center() - (*m).boundingBox.center()).length();
if (smallestDist < 0 || dist < smallestDist)
{
smallestDist = dist;
bestPartner = m;
}

}
return bestPartner;
}
*/

/*
vector<Mesh>::iterator findBestPartner(GeoContainer& container)
{
int smallestDist = -1;
int dist;
vector<Mesh>::iterator bestPartner;
for (vector<Mesh>::iterator m = meshes.begin(); m != meshes.end(); m++)
{
dist = fabs(container.boundingBox.merged((*m).boundingBox).volume());
if (smallestDist < 0 || dist < smallestDist)
{
smallestDist = dist;
bestPartner = m;
}

}
return bestPartner;
}
*/