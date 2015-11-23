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

	~Screen()
	{
	}
};

