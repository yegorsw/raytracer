#pragma once
class Color
{
public:
	double r = 0;
	double g = 0;
	double b = 0;

	//default constructor
	Color()
	{
	}

	//contructor
	Color(double rIn, double gIn, double bIn)
	{
		r = rIn;
		g = gIn;
		b = bIn;
	}

	~Color()
	{
	}
};

