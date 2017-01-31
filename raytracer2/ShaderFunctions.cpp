#include <math.h>
#include "Vec.h"
double fresnel(double dotProduct, double iorCurrent, double iorNext)
{
	double r = (iorCurrent - iorNext) / (iorCurrent + iorNext);
	r = r * r;
	r = r + (1.0 - r) * pow(1.0 - fabs(dotProduct), 5);

	return r;
}

void transformToWorldNormal(Vec & v, Vec & xvec, Vec & yvec, Vec & zvec, double r, double phi)
{

}