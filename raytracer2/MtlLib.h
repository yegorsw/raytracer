#pragma once
#include <vector>
#include "Mtl.h"

class MtlLib
{
private:
	
public:
	vector<Mtl> mtlList{ Mtl() };

	MtlLib()
	{
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

	void printToConsole()
	{
		cout << "Material Library:" << endl;
		for (vector<Mtl>::iterator i = mtlList.begin(); i < mtlList.end(); i++)
		{
			cout << i->name << endl;
		}
	}

	~MtlLib()
	{
	}
};

