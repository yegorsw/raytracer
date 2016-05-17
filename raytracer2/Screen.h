#pragma once
#include "Pixel.h"
#include <vector>

using namespace std;

class Screen
{
private:
	vector< vector<Pixel> > image;

public:
	
	int xres, yres;

	Screen(int xresIn, int yresIn)
	{
		xres = xresIn;
		yres = yresIn;

		//create empty screen of pixels
		for (int x = 0; x < xres; x++){
			vector<Pixel> row;
			for (int y = 0; y < yres; y++){
				row.push_back(Pixel());
			}
			image.push_back(row);
		}
	}

	Pixel getPixel(int x, int y)
	{
		return (image[x][y]);
	}

	void setValues(int x, int y, double rIn, double gIn, double bIn, double aIn)
	{
		image[x][y].setValues(rIn, gIn, bIn, aIn);
	}

	void setPixel(int x, int y, Pixel pixel)
	{
		image[x][y] = pixel;
	}

	void invertRGB()
	{
		for (int x = 0; x < xres; x++)
		{
			for (int y = 0; y < yres; y++)
			{
				image[x][y].color.r = 1.0 - image[x][y].color.r;
				image[x][y].color.g = 1.0 - image[x][y].color.g;
				image[x][y].color.b = 1.0 - image[x][y].color.b;
			}
		}
	}

	void constantAlpha()
	{
		for (int x = 0; x < xres; x++)
		{
			for (int y = 0; y < yres; y++)
			{
				image[x][y].a = 1.0;
			}
		}
	}

	void normalizeValues(bool individual = true)
	{
		double maxr = 0.0;
		double maxg = 0.0;
		double maxb = 0.0;
		for (int x = 0; x < xres; x++)
		{
			for (int y = 0; y < yres; y++)
			{
				maxr = max(maxr, image[x][y].color.r);
				maxg = max(maxg, image[x][y].color.g);
				maxb = max(maxb, image[x][y].color.b);
			}
		}

		
		double maxv = max(maxr, max(maxg, maxb));
		for (int x = 0; x < xres; x++)
		{
			for (int y = 0; y < yres; y++)
			{
				if (individual)
				{
					image[x][y].color.r /= maxr;
					image[x][y].color.g /= maxg;
					image[x][y].color.b /= maxb;
				}
				else
				{
					image[x][y].color.r /= maxv;
					image[x][y].color.g /= maxv;
					image[x][y].color.b /= maxv;
				}
			}
		}

	}

	~Screen()
	{
	}
};

