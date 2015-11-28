#pragma once
#include "Vec.h"
#include "Ray.h"

class Tri
{
public:
	//vertex positions
	Vec p0;
	Vec p1;
	Vec p2;

	//point normals (read from file)
	Vec n0;
	Vec n1;
	Vec n2;

	//geometric triangle normal
	Vec n;

	Vec edge01;
	Vec edge02;

	Tri()
	{
	}

	Tri(Vec p0in, Vec p1in, Vec p2in)
	{
		p0 = p0in;
		p1 = p1in;
		p2 = p2in;

		edge01 = p1 - p0;
		edge02 = p2 - p0;

		n = edge01.cross(edge02);
		n.normalize();
	}

	void setNormals(Vec n0In, Vec n1In, Vec n2In)
	{
		n0 = n0In;
		n1 = n1In;
		n2 = n2In;
	}

	void setNormalsToFace()
	{
		n0 = n;
		n1 = n;
		n2 = n;
	}

	float minX()
	{
		return min(min(p0.x, p1.x), p2.x);
	}

	float minY()
	{
		return min(min(p0.y, p1.y), p2.y);
	}

	float minZ()
	{
		return min(min(p0.z, p1.z), p2.z);
	}

	float maxX()
	{
		return max(max(p0.x, p1.x), p2.x);
	}

	float maxY()
	{
		return max(max(p0.y, p1.y), p2.y);
	}

	float maxZ()
	{
		return max(max(p0.z, p1.z), p2.z);
	}

	double intersect(Ray& ray)
	{
		if (ray.dir.dot(n) >= 0)
			return 0.0;

		Vec pvec = ray.dir.cross(edge02);
		double det = edge01.dot(pvec);
		
		//if ray is parallel with triangle
		if (det > -0.000001 && det < 0.000001)
			return 0.0;
		
		double invDet = 1.0 / det;

		//line from triangle origin (p0) to ray origin
		Vec tvec = ray.p - p0;

		//barycentric coord relative to p1
		double b1 = tvec.dot(pvec) * invDet;

		//if hit point is NOT on inner side of edge 0-2 OR if it is past vertex b1
		if (b1 < 0 || b1 > 1)
			return 0.0;

		//perpendicular to p0-rayOrigin & p0-p1 vectors
		Vec qvec = tvec.cross(edge01);

		//barycentric coord relative to p2
		double b2 = ray.dir.dot(qvec) * invDet;

		if (b2 < 0 || b1 + b2 > 1)
			return 0.0;

		double t = edge02.dot(qvec) * invDet;

		return t;
	}

	void printToConsole(){
		cout << "Triangle:\n" << '\t';
		p0.printToConsole();
		cout << '\n' << '\t';
		p1.printToConsole();
		cout << '\n' << '\t';
		p2.printToConsole();
		cout << '\n';
	}

	~Tri()
	{
	}
};

