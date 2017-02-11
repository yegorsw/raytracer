#pragma once

#define PI 3.1415926535897932

double lerp(double a, double b, double mix);
double length(double a, double b);
double randf();
double randfneg();
int fastrand(int seed);
double haltonRand(int i, int base);
void squareToDisk(double& x, double& y);
