#pragma once

#include "Tri.h"
#include "BBox.h"
#include "globals.h"
#include "ymath.h"

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
	}

	double cost()
	{
		return boundingBox.area() * numberOfTris() * 0.0001;
	}

	double calculateSplitCost(double splitPos, int axis)
	{
		double splitCost;
		GeoContainer dummy1, dummy2;

		double splitPosWorldSpace;
		splitPosWorldSpace = lerp(boundingBox.minCoord.p[axis], boundingBox.maxCoord.p[axis], splitPos);

		for (vector<Tri*>::iterator t = tris.begin(); t != tris.end(); t++)
		{
			if ((**t).p0.p[axis] < splitPosWorldSpace || (**t).p1.p[axis] < splitPosWorldSpace || (**t).p2.p[axis] < splitPosWorldSpace)
				dummy1.addTri(*t);

			if ((**t).p0.p[axis] > splitPosWorldSpace || (**t).p1.p[axis] > splitPosWorldSpace || (**t).p2.p[axis] > splitPosWorldSpace)
				dummy2.addTri(*t);
		}
		
		dummy1.boundingBox = boundingBox;
		dummy1.boundingBox.maxCoord.p[axis] = splitPosWorldSpace;

		dummy2.boundingBox = boundingBox;
		dummy2.boundingBox.minCoord.p[axis] = splitPosWorldSpace;

		if (dummy1.numberOfTris() == 0 || dummy2.numberOfTris() == 0)
			return cost() + 1.0;

		dummy1.generateBbox(true);
		dummy2.generateBbox(true);

		return dummy1.cost() + dummy2.cost();
	}

	void split(double splitPos, int axis)
	{
		child1 = new GeoContainer();
		child2 = new GeoContainer();

		double splitPosWorldSpace;
		splitPosWorldSpace = lerp(boundingBox.minCoord.p[axis], boundingBox.maxCoord.p[axis], splitPos);

		child1->boundingBox = boundingBox;
		child1->boundingBox.maxCoord.p[axis] = splitPosWorldSpace;

		child2->boundingBox = boundingBox;
		child2->boundingBox.minCoord.p[axis] = splitPosWorldSpace;

		for (vector<Tri*>::iterator t = tris.begin(); t != tris.end(); t++)
		{
			if ((**t).p0.p[axis] < splitPosWorldSpace || (**t).p1.p[axis] < splitPosWorldSpace || (**t).p2.p[axis] < splitPosWorldSpace)
			{
				child1->addTri(*t);
			}
			if ((**t).p0.p[axis] >= splitPosWorldSpace || (**t).p1.p[axis] >= splitPosWorldSpace || (**t).p2.p[axis] >= splitPosWorldSpace)
			{
				child2->addTri(*t);
			}
		}
		
		tris = vector<Tri*>();

		if (child1->numberOfTris() > 0)
			child1->generateBbox(true);

		if (child2->numberOfTris() > 0)
			child2->generateBbox(true);
	}

	void buildBboxHierarchy(int depth = 0)
	{
		if(depth == 0) 	cout << "Building BVH tree" << endl;

		if (depth < 50 && numberOfTris() > 10)
		{
			if (depth < 4) cout << string(depth, ' ') << "depth " << depth << endl;

			//find best split axis and position
			int bestSplitAxis;
			double bestSplitPos;

			double bestSplitCost = cost();
			double splitCost;

			bool dosplit = false;

			//double stepsize = 1.0 / sqrt(numberOfTris());
			double stepsize = 0.1;
			for (double splitpos = stepsize; splitpos < 1.0; splitpos += stepsize)
			{
				for (int axis = 0; axis < 3; axis++)
				{
					splitCost = calculateSplitCost(splitpos, axis);

					if (splitCost < bestSplitCost)
					{
						bestSplitCost = splitCost;
						bestSplitAxis = axis;
						bestSplitPos = splitpos;
						dosplit = true;
					}
				}
			}

			//do split
			if (dosplit)
			{
				split(bestSplitPos, bestSplitAxis);
				#pragma omp parallel sections // starts a new team
				{
					#pragma omp section
					{child1->buildBboxHierarchy(depth + 1); }

					#pragma omp section
					{child2->buildBboxHierarchy(depth + 1); }
				}
			}
		}
	}

	bool intersect(Ray& ray, Tri*& closestTri, double& shortestDist, int& numBoxIntersections, int depth = 0)
	{
		bool intersection = false;


		//intersect all tris first
		if (numberOfTris() > 0)
		{
			double dist;
			for (vector<Tri*>::iterator t = tris.begin(); t != tris.end(); t++)
			{
				if ((**t).intersect(ray, dist))
				{
					if (dist < shortestDist)
					{
						intersection = true;
						closestTri = *t;
						shortestDist = dist;
					}
				}
			}
		}

		double child1Dist = SCN_MAXDIST;
		double child2Dist = SCN_MAXDIST;

		bool child1Hit = false;
		bool child2Hit = false;

		GeoContainer *closerChild, *furtherChild;

		if (child1 && child1->boundingBox.intersect(ray, child1Dist) && child1Dist < shortestDist)
		{
			child1Hit = true;
			numBoxIntersections++;
		}

		if (child2 && child2->boundingBox.intersect(ray, child2Dist) && child2Dist < shortestDist)
		{
			child2Hit = true;
			numBoxIntersections++;
		}

		if (child1Hit && child2Hit)
		{
			if (child1Dist < child2Dist)
			{
				intersection = child1->intersect(ray, closestTri, shortestDist, numBoxIntersections, depth + 1) || intersection;
				if (child2Dist < shortestDist)
					intersection = child2->intersect(ray, closestTri, shortestDist, numBoxIntersections, depth + 1) || intersection;
			}
			else
			{
				intersection = child2->intersect(ray, closestTri, shortestDist, numBoxIntersections, depth + 1) || intersection;
				if (child1Dist < shortestDist)
					intersection = child1->intersect(ray, closestTri, shortestDist, numBoxIntersections, depth + 1) || intersection;
			}
		}
		else if (child1Hit)
			intersection = child1->intersect(ray, closestTri, shortestDist, numBoxIntersections, depth + 1) || intersection;
		else if (child2Hit)
			intersection = child2->intersect(ray, closestTri, shortestDist, numBoxIntersections, depth + 1) || intersection;

		return intersection;
	}

	int numberOfTris(bool recurse = false)
	{
		int numTris = tris.size();
		if (recurse)
		{
			if (child1)
				numTris += child1->numberOfTris(true);

			if (child2)
				numTris += child2->numberOfTris(true);
		}

		return numTris;
	}

	void printToConsole(int depth = 0)
	{
		cout << endl << string(depth * 3, ' ');
		boundingBox.minCoord.printToConsole();
		cout << endl << string(depth * 3, ' ');
		boundingBox.maxCoord.printToConsole();

		cout << endl << string(depth * 3, ' ');
		cout << numberOfTris();

		cout << endl;

		if (child1)
			child1->printToConsole(depth + 1);

		if (child2)
			child2->printToConsole(depth + 1);

		//if (depth == 0)
		//	cout << numberOfTris() << " total tris" << endl;

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