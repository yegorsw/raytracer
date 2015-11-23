#pragma once
#include "Color.h"

class Pixel
{
public:
	Color color;
	double a = 0;

	//defalt contructor
	Pixel()
	{
	}

	//--definitions

	//contructor
	Pixel(double rIn, double gIn, double bIn, double aIn)
	{
		color.r = rIn;
		color.g = gIn;
		color.b = bIn;
		a = aIn;
	}

	//set values of pixel
	void setValues(double rIn, double gIn, double bIn, double aIn)
	{
		color.r = rIn;
		color.g = gIn;
		color.b = bIn;
		a = aIn;
	}

	void setColor(double rIn, double gIn, double bIn)
	{
		color.r = rIn;
		color.g = gIn;
		color.b = bIn;
	}

	void setAlpha(double aIn)
	{
		a = aIn;
	}

	void operator+=(Pixel &p)
	{
		color.r += p.color.r;
		color.g += p.color.g;
		color.b += p.color.b;
		a += p.a;
	}

	void operator-=(Pixel &p)
	{
		color.r -= p.color.r;
		color.g -= p.color.g;
		color.b -= p.color.b;
		a -= p.a;
	}

	void operator+=(double n)
	{
		color.r += n;
		color.g += n;
		color.b += n;
		a += n;
	}

	void operator*=(double n)
	{
		color.r *= n;
		color.g *= n;
		color.b *= n;
		a *= n;
	}

	void operator/=(double n)
	{
		double inv = 1.0 / n;
		color.r *= inv;
		color.g *= inv;
		color.b *= inv;
		a *= inv;
	}

	double magnitude()
	{
		//return max(max(color.r, color.g), max(color.b, a));
		return max(
			max(fabs(color.r), fabs(color.g)), 
			max(fabs(color.b), fabs(a))
			);
	}

	void printToConsole(){
		cout << '[' << color.r << ", " << color.g << ", " << color.b << ", " << a << ']' << endl;
	}

	~Pixel()
	{
	}
};

