#pragma once
#include "Vec.h"
#include "Ray.h"
#include "Mtl.h"

#include "globals.h"

class Tri
{
private:
	Vec edge01;
	Vec edge02;
	

public:
	Mtl* mtl;

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

	Tri()
	{
	}

	Tri(Vec p0in, Vec p1in, Vec p2in)
	{
		p0 = p0in;
		p1 = p1in;
		p2 = p2in;
	}

	void precompute()
	{
		edge01 = p1 - p0;
		edge02 = p2 - p0;

		n = edge01.cross(edge02);
		n.normalize();
	}

	void setPoints(Vec p0In, Vec p1In, Vec p2In)
	{
		p0 = p0In;
		p1 = p1In;
		p2 = p2In;
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

	double minX()
	{
		return min(min(p0.p[0], p1.p[0]), p2.p[0]);
	}

	double minY()
	{
		return min(min(p0.p[1], p1.p[1]), p2.p[1]);
	}

	double minZ()
	{
		return min(min(p0.p[2], p1.p[2]), p2.p[2]);
	}

	double maxX()
	{
		return max(max(p0.p[0], p1.p[0]), p2.p[0]);
	}

	double maxY()
	{
		return max(max(p0.p[1], p1.p[1]), p2.p[1]);
	}

	double maxZ()
	{
		return max(max(p0.p[2], p1.p[2]), p2.p[2]);
	}

	Vec center()
	{
		return (p0 + p1 + p2) * 0.33333333333333333333333333;
	}

	bool intersect(Ray& ray, double& dist)
	{
		Vec pvec = ray.dir.cross(edge02);
		double det = edge01.dot(pvec);
		
		g_triIntersections++;

		//if ray is parallel with triangle
		if (det < 0.000001 && det > -0.000001)// uncomment to disable backface culling
			return false;
		
		double invDet = 1.0 / det;

		//line from triangle origin (p0) to ray origin
		Vec tvec = ray.pos - p0;

		//barycentric coord relative to p1
		double b1 = tvec.dot(pvec) * invDet;

		//if hit point is NOT on inner side of edge 0-2 OR if it is past vertex b1
		if (b1 < 0 || b1 > 1)
			return false;

		//perpendicular to p0-rayOrigin & p0-p1 vectors
		Vec qvec = tvec.cross(edge01);

		//barycentric coord relative to p2
		double b2 = ray.dir.dot(qvec) * invDet;

		if (b2 < 0 || b1 + b2 > 1)
			return false;

	    dist = edge02.dot(qvec) * invDet;

		return true;
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

