#include <stdlib.h>
#include <math.h>
#include "Vec.h"

using namespace std;

double lerp(double a, double b, double mix)
{
	return a * (1.0 - mix) + b * (mix);
}

double length(double a, double b)
{
	return sqrt(a * a + b * b);
}

double randf()
{
	return rand() * (1.0 / (double)RAND_MAX);
}

double randfneg()
{
	return (rand() * (1.0 / (double)RAND_MAX) * 2.0) - 1.0;
}

