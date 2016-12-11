#pragma once

#include "Tri.h"
#include "BBox.h"
#include "globals.h"

class GeoContainer
{
public:
	BBox boundingBox;
	vector<Tri*> tris;
	GeoContainer* child1;
	GeoContainer* child2;
	GeoContainer* parent;

	GeoContainer()
	{
	}

	void addTri(Tri* t)
	{
		tris.push_back(t);
	}

	void generateBbox(bool shrinkWrap = false)
	{

		double minX = tris[0]->p0.p[0];
		double minY = tris[0]->p0.p[1];
		double minZ = tris[0]->p0.p[2];

		double maxX = tris[0]->p0.p[0];
		double maxY = tris[0]->p0.p[1];
		double maxZ = tris[0]->p0.p[2];

		for (int i = 0; i < tris.size(); i++)
		{
			minX = min(minX, tris[i]->minX());
			minY = min(minY, tris[i]->minY());
			minZ = min(minZ, tris[i]->minZ());

			maxX = max(maxX, tris[i]->maxX());
			maxY = max(maxY, tris[i]->maxY());
			maxZ = max(maxZ, tris[i]->maxZ());
		}

		if (shrinkWrap)
		{
			minX = max(minX, boundingBox.minCoord.p[0]);
			minY = max(minY, boundingBox.minCoord.p[1]);
			minZ = max(minZ, boundingBox.minCoord.p[2]);

			maxX = min(maxX, boundingBox.maxCoord.p[0]);
			maxY = min(maxY, boundingBox.maxCoord.p[1]);
			maxZ = min(maxZ, boundingBox.maxCoord.p[2]);

			boundingBox.minCoord = Vec(minX, minY, minZ);
			boundingBox.maxCoord = Vec(maxX, maxY, maxZ);
		}
		else {
			boundingBox = BBox(Vec(minX, minY, minZ), Vec(maxX, maxY, maxZ));
		}


		if (child1)
			boundingBox = boundingBox.merged(child1->boundingBox);

		if (child2)
			boundingBox = boundingBox.merged(child2->boundingBox);
	}

	double calculateSplitCost(double splitPos, char splitAxis)
	{
		return 0;
	}

	void split(double splitPos, char splitAxis)
	{
		child1 = new GeoContainer();
		child2 = new GeoContainer();

		double splitPosWorldSpace;

		for (int i = 0; i < 3; i++)
		{
			splitPosWorldSpace = (boundingBox.minCoord * (1.0 - splitPos) + boundingBox.maxCoord * (splitPos)).p[0];

		}
	}

	void buildBboxHierarchy(int depth = 0)
	{

	}

	bool intersect(Ray& ray, Tri*& closestTri, double& shortestDist, int depth = 0)
	{
		//intersect all tris first
		double dist;
		double prevDist = shortestDist;
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
		if (child1 && child2)
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

		return shortestDist < prevDist;
	}

	int numberOfTris()
	{
		int numTris = tris.size();

		if (child1)
			numTris += child1->numberOfTris();

		if (child2)
			numTris += child2->numberOfTris();

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


/*
buildbboxhierarchy:...
bestcost = 9999
stepsize = 0.1

for i = stepsize; i < 1; i++
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
if ((**m).boundingBox.center()[0] <= center()[0])
moveMeshToGroup(m, *child1);
else
moveMeshToGroup(m, *child2);
}
else if (boundingBox.largestAxis() == 1)
{
if ((**m).boundingBox.center()[1] <= center()[1])
moveMeshToGroup(m, *child1);
else
moveMeshToGroup(m, *child2);
}
else if (boundingBox.largestAxis() == 2)
{
if ((**m).boundingBox.center()[2] <= center()[2])
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