#include <stdlib.h>
#include <math.h>
#include "Vec.h"
#include "ymath.h"

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

int fastrand(int seed)
{
	for (int i = 0; i < 5; i++)
	{
		seed = (214013 * seed + 2531011);
		seed = (seed >> 16) & 0x7FFF;
	}
	return seed;
}

double haltonRand(int i, int base)
{
	double result = 0;
	double f = 1;
	while (i > 0)
	{
		f /= double(base);
		result += f * (i % base);
		i /= base;
	}

	return result;
}

//http://psgraphics.blogspot.ca/2011/01/improved-code-for-concentric-map.html
void squareToDisk(double & x, double & y)
{
	x = 2 * x - 1;
	y = 2 * x - 1;

	if (x != 0 && y != 0)
	{
		double r, phi;

		if (fabs(x) > fabs(y))
		{
			r = x;
			phi = PI / 4 * y / x;
		}
		else
		{
			r = y;
			phi = PI / 2 - PI / 4 * x / y;
		}

		x = r * cos(phi);
		y = r * sin(phi);
	}
}

