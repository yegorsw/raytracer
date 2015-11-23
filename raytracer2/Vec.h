#pragma once
#include <algorithm>
#include <iostream>//for debugging

using namespace std;

class Vec
{
public:
	double x = 0;
	double y = 0;
	double z = 0;

	//default constructor
	Vec()
	{
	}

	Vec(double xIn, double yIn, double zIn)
	{
		x = xIn;
		y = yIn;
		z = zIn;
	}

	void setValues(double xIn, double yIn, double zIn)
	{
		x = xIn;
		y = yIn;
		z = zIn;
	}

	double length()
	{
		return sqrt(x*x + y*y + z*z);
	}

	void normalize()
	{
		if (x == 0.0 && y == 0.0 && z == 0.0)
		{
			x = 1.0;
		}
		else
		{
			double l = 1.0 / length();
			x *= l;
			y *= l;
			z *= l;
		}
	}

	double dot(Vec &p)
	{
		return (x * p.x) + (y * p.y) + (z * p.z);
	}

	Vec cross(Vec &p)
	{
		return Vec(
			y * p.z - z * p.y,
			z * p.x - x * p.z,
			x * p.y - y * p.x
			);
	}

	Vec operator+(Vec &v)
	{
		return Vec(
			x + v.x,
			y + v.y,
			z + v.z
			);
	}

	Vec operator-(Vec &v)
	{
		return Vec(
			x - v.x,
			y - v.y,
			z - v.z
			);
	}

	Vec operator*(double n)
	{
		return Vec(
			x * n,
			y * n,
			z * n
			);
	}

	Vec operator/(double n)
	{
		n = 1.0 / n;;
		return Vec(
			x * n,
			y * n,
			z * n
			);
	}

	void printToConsole(){
		cout << '[' << x << ", " << y << ", " << z << ']' << endl;
	}

	~Vec()
	{
	}
};

