#pragma once

#include "Tri.h"
#include "BBox.h"
#include "globals.h"

class GeoContainer
{
public:
	BBox boundingBox;
	//vector<Mesh*> meshes;
	vector<Tri*> tris;
	GeoContainer* child1;
	GeoContainer* child2;
	GeoContainer* parent;
	bool hasChildren = false;

	GeoContainer()
	{
	}

	void addTri(Tri* t)
	{
		tris.push_back(t);
	}

	void generateBbox()
	{
		boundingBox = BBox();
		float minX = tris[0]->p0.x;
		float minY = tris[0]->p0.y;
		float minZ = tris[0]->p0.z;

		float maxX = tris[0]->p0.x;
		float maxY = tris[0]->p0.y;
		float maxZ = tris[0]->p0.z;

		for (int i = 0; i < tris.size(); i++)
		{
			minX = min(minX, tris[i]->minX());
			minY = min(minY, tris[i]->minY());
			minZ = min(minZ, tris[i]->minZ());

			maxX = max(maxX, tris[i]->maxX());
			maxY = max(maxY, tris[i]->maxY());
			maxZ = max(maxZ, tris[i]->maxZ());
		}

		boundingBox = {
			Vec(minX, minY, minZ),
			Vec(maxX, maxY, maxZ)
		};
		if (child1)
			boundingBox = boundingBox.merged(child1->boundingBox);

		if (child2)
			boundingBox = boundingBox.merged(child2->boundingBox);
	}

	Vec center()
	{
		Vec mid;
		mid += child1->boundingBox.center();
		mid += child2->boundingBox.center();
		mid = mid / 2.0;

		return mid;
	}

	void buildBboxHierarchy(int depth = 0)
	{

	}

	/*
	buildbboxhierarchy:
		bestcost = 9999
		for i = stepsize; i < 1.0 - stepsize; i++
			getCost(
	*/
	
	/*void buildBboxHierarchy(int depth=0)
	{
		generateBbox();

		if (numberOfMeshes() > 3 && depth < 8)
		{

			if (depth < 30)
				cout << string(depth*2, ' ') << "Sorting Depth " << depth << endl;

			GeoContainer* child1 = new GeoContainer;
			GeoContainer* child2 = new GeoContainer;

			for (vector<Mesh*>::iterator m = meshes.begin(); m != meshes.end();)
			{
				if (boundingBox.largestAxis() == 0)
				{
					if ((**m).boundingBox.center().x <= center().x)
						moveMeshToGroup(m, *child1);
					else
						moveMeshToGroup(m, *child2);
				}
				else if (boundingBox.largestAxis() == 1)
				{
					if ((**m).boundingBox.center().y <= center().y)
						moveMeshToGroup(m, *child1);
					else
						moveMeshToGroup(m, *child2);
				}
				else if (boundingBox.largestAxis() == 2)
				{
					if ((**m).boundingBox.center().z <= center().z)
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

		
		for (vector<Mesh>::iterator m = meshes.begin(); m != meshes.end(); m++)
		{
			(*m).buildBboxHierarchy();
		}

	}*/

	bool intersect(Ray& ray, Tri*& closestTri, double& shortestDist, int depth = 0)
	{
		//intersect all tris first
		double dist;
		for (vector<Tri*>::iterator t = tris.begin(); t != tris.end(); t++)
		{
			if ((**t).intersect(ray, dist))
			{
				if (dist < shortestDist)
				{
					closestTri = *t;
					shortestDist = dist;
				}
			}
		}

		//if has children, intersect them
		if (hasChildren)
		{
			double dist1, dist2;
			bool hit1 = child1->boundingBox.intersect(ray, dist1);
			bool hit2 = child2->boundingBox.intersect(ray, dist2);
			
			//if ray passes through both bounding boxes
			if (hit1 && hit2)
			{
				//intersect closer child first
				if (dist1 < dist2)
				{
					if (dist1 < shortestDist)
						child1->intersect(ray, closestTri, shortestDist, depth + 1);

					if (dist2 < shortestDist)
						child2->intersect(ray, closestTri, shortestDist, depth + 1);
				}
				else
				{
					if (dist2 < shortestDist)
						child2->intersect(ray, closestTri, shortestDist, depth + 1);

					if (dist1 < shortestDist)
						child1->intersect(ray, closestTri, shortestDist, depth + 1);
				}

			}
			//if ray only passes through child1
			else if (hit1 && !hit2)
			{
				if (dist1 < shortestDist)
					child1->intersect(ray, closestTri, shortestDist, depth + 1);
			}
			//if ray only passes through child2
			else if (hit2 && !hit1)
			{
				if (dist2 < shortestDist)
					child1->intersect(ray, closestTri, shortestDist, depth + 1);
			}
			
		}

		return (shortestDist > 0.0 && shortestDist < 4999);
	}

	int numberOfTris()
	{
		int numTris = tris.size();
//		numTris += child1->numberOfTris();
//		numTris += child2->numberOfTris();

		return numTris;
	}

	void printToConsole(int depth = 0)
	{
		cout << endl << string(depth * 3, ' ');
		boundingBox.minCoord.printToConsole();
		cout << endl << string(depth * 3, ' ');
		boundingBox.maxCoord.printToConsole();

		cout << endl;

		child1->printToConsole(depth + 1);
		child2->printToConsole(depth + 1);

		//if (depth == 0)
			cout << numberOfTris() << " total tris" << endl;

	}



	~GeoContainer()
	{
	}
};