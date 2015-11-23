#pragma once
#include "Color.h"
#include <string>

using namespace std;

class Mtl
{
public:
	Color Kd;
	Color Ka;
	string name;

	Mtl()
	{
	}

	Mtl(string nameIn, Color KdIn, Color KaIn)
	{
		name = nameIn;
		Kd = KdIn;
		Ka = KaIn;
	}

	~Mtl()
	{
	}
};

