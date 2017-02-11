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

}