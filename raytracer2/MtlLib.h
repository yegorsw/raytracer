#pragma once
#include <vector>
#include "Mtl.h"

class MtlLib
{
private:
	vector<Mtl> mtlList;
public:
	MtlLib()
	{
		Mtl defaultMtl(
			"default", 
			Color{ 0.18, 0.18, 0.18 }, 
			Color{ 0, 0, 0 }
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

	~MtlLib()
	{
	}
};

