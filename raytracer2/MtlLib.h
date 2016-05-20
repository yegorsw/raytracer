#pragma once
#include <vector>
#include "Mtl.h"

class MtlLib
{
private:
	
public:
	vector<Mtl> mtlList;
	MtlLib()
	{
		Mtl defaultMtl(
			"default", 
			Color{ 0.18, 0.18, 0.18 }, 
			Color{ 1.0, 0.0, 1.0 }
		);
		mtlList.push_back(defaultMtl);
	}

	void addMtl(Mtl mtl)
	{
		mtlList.push_back(mtl);
	}

	Mtl& getMtl(int i)
	{
		return mtlList[i];
	}

	int numberOfMtls()
	{
		return mtlList.size();
	}

	Mtl* findMtl(string mtlName)
	{
		for (int i = 0; i < numberOfMtls(); i++)
		{
			if (mtlList[i].name == mtlName)
			{
				return &mtlList[i];
			}
		}

		return &mtlList[0];
	}

	~MtlLib()
	{
	}
};

