#include <math.h>
#include "Vec.h"
#include "ShaderFunctions.h"
double fresnel(double dotProduct, double iorCurrent, double iorNext)
{
	double r = (iorCurrent - iorNext) / (iorCurrent + iorNext);
	r = r * r;
	r = r + (1.0 - r) * pow(1.0 - fabs(dotProduct), 5);

	return r;
}

Vec reflect(Vec& dir, Vec& normal)
{
	return (normal * (-dir).dot(normal) * 2.0) + dir;
}

Vec refract(Vec& dir, Vec& normal, double iorCurrent, double iorNext)
{
	double eta = iorCurrent / iorNext;
	double facingRatio = dir.dot(normal);
	double k = 1.0 - eta*eta * (1.0 - facingRatio * facingRatio);
	Vec d = (dir*0.8 + normal*0.2);
	d.normalize();
	return d;
	return (dir * eta) - (normal * (dir * eta - normal * (eta * facingRatio + sqrt(k))));
}