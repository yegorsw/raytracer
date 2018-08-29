#pragma once
#include <algorithm>
#include <iostream>//for debugging

using namespace std;

class Vec
{
public:
	double p[3] = { 0.0, 0.0, 0.0 };

	//default constructor
	Vec()
	{
	}

	Vec(double xIn, double yIn, double zIn)
	{
		p[0] = xIn;
		p[1] = yIn;
		p[2] = zIn;
	}

	void setValues(double xIn, double yIn, double zIn)
	{
		p[0] = xIn;
		p[1] = yIn;
		p[2] = zIn;
	}

	double squaredLength()
	{
		return p[0] * p[0] + p[1] * p[1] + p[2] * p[2];
	}

	double length()
	{
		return sqrt(squaredLength());
	}

	void normalize()
	{
		if (p[0] == 0.0 && p[1] == 0.0 && p[2] == 0.0)
		{
			p[0] = 1.0;
		}
		else
		{
			double l = 1.0 / length();
			p[0] *= l;
			p[1] *= l;
			p[2] *= l;
		}
	}

	Vec inverse()
	{
		return Vec(1.0 / p[0], 1.0 / p[1], 1.0 / p[2]);
	}

	void transformTo(Vec& xvec, Vec& yvec, Vec& zvec)
	{
		double x = p[0];
		double y = p[1];
		double z = p[2];
		Vec v = (xvec * x + yvec * y + zvec * z);
		p[0] = v.p[0];
		p[1] = v.p[1];
		p[2] = v.p[2];
	}

	double dot(Vec &v)
	{
		return (p[0] * v.p[0]) + (p[1] * v.p[1]) + (p[2] * v.p[2]);
	}

	Vec cross(Vec &v)
	{
		return Vec(
			p[1] * v.p[2] - p[2] * v.p[1],
			p[2] * v.p[0] - p[0] * v.p[2],
			p[0] * v.p[1] - p[1] * v.p[0]
			);
	}

	Vec operator+(Vec &v)
	{
		return Vec(
			p[0] + v.p[0],
			p[1] + v.p[1],
			p[2] + v.p[2]
			);
	}

	Vec operator-(Vec &v)
	{
		return Vec(
			p[0] - v.p[0],
			p[1] - v.p[1],
			p[2] - v.p[2]
			);
	}

	Vec operator*(Vec &v)
	{
		return Vec(
			p[0] * v.p[0],
			p[1] * v.p[1],
			p[2] * v.p[2]
		);
	}
	
	void operator-=(Vec &v)
	{
		p[0] -= v.p[0];
		p[1] -= v.p[1];
		p[2] -= v.p[2];
	}

	void operator+=(Vec &v)
	{
		p[0] += v.p[0];
		p[1] += v.p[1];
		p[2] += v.p[2];
	}

	Vec operator-()
	{
		return Vec(-p[0], -p[1], -p[2]);
	}

	Vec operator*(double n)
	{
		return Vec(
			p[0] * n,
			p[1] * n,
			p[2] * n
			);
	}

	Vec operator/(double n)
	{
		n = 1.0 / n;;
		return Vec(
			p[0] * n,
			p[1] * n,
			p[2] * n
			);
	}

	Vec minVec(Vec &v)
	{
		return Vec(min(p[0], v.p[0]), min(p[1], v.p[1]), min(p[2], v.p[2]));
	}

	Vec maxVec(Vec &v)
	{
		return Vec(max(p[0], v.p[0]), max(p[1], v.p[1]), max(p[2], v.p[2]));
	}

	void printToConsole(){
		cout << '[' << p[0] << ", " << p[1] << ", " << p[2] << ']' << endl;
	}

	~Vec()
	{
	}
};

inline void generateBasisFromZ(Vec& xaxis, Vec& yaxis, Vec& zaxis)
{
	xaxis = zaxis.p[0] > 0.9 ? Vec(0, 1, 0) : Vec(1, 0, 0);
	xaxis = zaxis.cross(xaxis);
	xaxis.normalize();
	yaxis = zaxis.cross(xaxis);
	yaxis.normalize();
}