#pragma once
double fresnel(double dotProduct, double iorCurrent, double iorNext);
Vec reflect(Vec& dir, Vec& normal);
Vec refract(Vec& dir, Vec& normal, double iorCurrent, double iorNext);