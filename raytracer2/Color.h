#pragma once
#include <algorithm>

class Color
{
public:
	double r = 0;
	double g = 0;
	double b = 0;

	//default constructor
	Color()
	{
	}

	//contructor
	Color(double rIn, double gIn, double bIn)
	{
		r = rIn;
		g = gIn;
		b = bIn;
	}

	Color(double v)
	{
		r = v;
		g = v;
		b = v;
	}

	void applyGamma(double gamma)
	{
		gamma = 1.0 / gamma;
		r = pow(r, gamma);
		g = pow(g, gamma);
		b = pow(b, gamma);
	}
	void clamp(double maximum)
	{
		r = r<maximum ? (r>0.0 ? r : 0.0) : maximum;
		g = g<maximum ? (g>0.0 ? g : 0.0) : maximum;
		b = b<maximum ? (b>0.0 ? b : 0.0) : maximum;
	}

	bool isBlack()
	{
		return r <= 0 && g <= 0 && b <= 0;
	}

	void operator*=(double n)
	{
		r *= n;
		g *= n;
		b *= n;
	}

	Color operator*(double n)
	{
		return Color(r * n, g * n, b * n);
	}

	void operator*=(Color& c)
	{
		r *= c.r;
		g *= c.g;
		b *= c.b;
	}

	void operator+=(Color& c)
	{
		r += c.r;
		g += c.g;
		b += c.b;
	}

	bool operator>(Color& c)
	{
		return r > c.r && g > c.g && b > c.b;
	}

	bool operator>(double v)
	{
		return r > v && g > v && b > v;
	}

	~Color()
	{
	}
};

