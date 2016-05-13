#pragma once
#include "Color.h"
#include "Tri.h"
#include <vector>
#include "BBox.h"
#include "renderOverrides.h"

using namespace std;

class Mesh
{
private:

public:
	BBox boundingBox;
	vector<Tri> triList;
	vector<Mesh> childMeshes;
	//constructor
	Mesh()
	{
	}

	int numberOfTris()
	{
		return triList.size();
	}

	void addTri(Tri tri)
	{
		triList.push_back(tri);
	}

	Tri getTri(int i)
	{
		return (triList[i]);
	}

	void generateBbox()
	{
		boundingBox = BBox();
		float minX = triList[0].p0.x;
		float minY = triList[0].p0.y;
		float minZ = triList[0].p0.z;

		float maxX = triList[0].p0.x;
		float maxY = triList[0].p0.y;
		float maxZ = triList[0].p0.z;

		for (int i = 0; i < triList.size(); i++)
		{
			minX = min(minX, triList[i].minX());
			minY = min(minY, triList[i].minY());
			minZ = min(minZ, triList[i].minZ());

			maxX = max(maxX, triList[i].maxX());
			maxY = max(maxY, triList[i].maxY());
			maxZ = max(maxZ, triList[i].maxZ());
		}

		boundingBox = { 
			Vec(minX, minY, minZ), 
			Vec(maxX, maxY, maxZ) 
		};
	}

	bool intersect(Ray& ray, Tri*& closestTri, double& shortestDist)
	{
		bool contains = boundingBox.contains(ray.p);
		double intersectDist = 0.0;

		if (!contains)
		{
			intersectDist = boundingBox.intersect(ray);
		}

		if (contains || (intersectDist > 0.0 && intersectDist < shortestDist)){
			for (int tri = 0; tri < numberOfTris(); tri++)
			{
				intersectDist = triList[tri].intersect(ray);

				if (intersectDist > 0.0 && intersectDist < shortestDist)
				{
					shortestDist = intersectDist;
					closestTri = &(triList[tri]);
				}
			}

			for (vector<Mesh>::iterator m = childMeshes.begin(); m != childMeshes.end(); m++)
			{
				(*m).intersect(ray, closestTri, shortestDist);
			}
		}

		return (shortestDist > 0.0 && shortestDist < 4999.9999);
	}

	void moveTriToGroup(vector<Tri>::iterator& t, Mesh& container)
	{
		move(t, t + 1, back_inserter(container.triList));

		//this automatically increments the mesh pointer to the next mesh
		triList.erase(t);
	}

	void buildBboxHierarchy(int depth = 0)
	{
		generateBbox();
		if (numberOfTris() > 3 && depth < 30)
		{
			if (depth < 4)
				cout << string(depth * 2, ' ') << "Sorting Mesh Depth " << depth << endl;
			
			Vec center;
			for (int tri = 0; tri < numberOfTris(); tri++)
			{
				center = center + triList[tri].center();
				
			}
			center = center / ((double)numberOfTris());

			Mesh* child1 = new Mesh;
			Mesh* child2 = new Mesh;
			for (vector<Tri>::iterator t = triList.begin(); t != triList.end();)
			{
				if (depth % 3 == 0)
				{
					if ((*t).center().x < center.x)
						moveTriToGroup(t, *child1);
					else
						moveTriToGroup(t, *child2);
				}
				else if (depth % 3 == 1)
				{
					if ((*t).center().y < center.y)
						moveTriToGroup(t, *child1);
					else
						moveTriToGroup(t, *child2);
				}
				else if (depth % 3 == 2)
				{
					if ((*t).center().z < center.z)
						moveTriToGroup(t, *child1);
					else
						moveTriToGroup(t, *child2);
				}

			}

			(*child1).buildBboxHierarchy(depth + 1);
			(*child2).buildBboxHierarchy(depth + 1);

			childMeshes.push_back(*child1);
			childMeshes.push_back(*child2);
		}
	}

	void clear()
	{
		triList.clear();
		//boundingBox = BBox();
	}
	
	~Mesh()
	{
	}
};

