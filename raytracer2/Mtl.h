#pragma once
#include "Color.h"
#include <string>

using namespace std;

class Mtl
{
public:
	Color Kd;
	Color Ka;
	string name = "";

	Mtl()
	{
		name = "default";
		Kd = Color{ 0.18, 0.18, 0.18 };
		Ka = Color{ 0.0, 0.0, 0.0 };
	}

	Mtl(string nameIn, Color KdIn, Color KaIn)
	{
		name = nameIn;
		Kd = KdIn;
		Ka = KaIn;
	}

	bool hasDiffuse()
	{
		return Kd.r + Kd.g + Kd.b > 0.000001;
	}

	~Mtl()
	{
	}
};

