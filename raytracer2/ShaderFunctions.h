#pragma once
double fresnel(double dotProduct, double iorCurrent, double iorNext);
void transformToWorldNormal(Vec & v, Vec & xvec, Vec & yvec, Vec & zvec, double r, double phi);