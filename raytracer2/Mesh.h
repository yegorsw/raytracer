#pragma once
#include "Color.h"
#include "Tri.h"
#include <vector>
#include "BBox.h"

using namespace std;

class Mesh
{
private:

public:
	BBox boundingBox;
	vector<Tri> triList;
	bool hasBbox = false;
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
		hasBbox = true;
	}

	void clear()
	{
		triList.clear();
		hasBbox = false;
		//boundingBox = BBox();
	}
	
	~Mesh()
	{
	}
};

