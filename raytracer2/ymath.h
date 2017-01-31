#pragma once

#define PI 3.14159265358979

double lerp(double a, double b, double mix);
double length(double a, double b);
double randf();
double randfneg();
double haltonRand(int i, int base);
void squareToDisk(double& x, double& y);
