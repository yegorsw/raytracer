#pragma once
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

	void operator*=(double n)
	{
		r *= n;
		g *= n;
		b *= n;
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

	void applyGamma(double gamma)
	{
		gamma = 1.0 / gamma;
		r = pow(r, gamma);
		g = pow(g, gamma);
		b = pow(b, gamma);
	}
	void clamp()
	{
		r = r<1.0 ? (r>0.0 ? r : 0.0) : 1.0;
		g = g<1.0 ? (g>0.0 ? g : 0.0) : 1.0;
		b = b<1.0 ? (b>0.0 ? b : 0.0) : 1.0;
	}
	~Color()
	{
	}
};

