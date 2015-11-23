#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <iostream>

#include "Scene.h"
#include "Screen.h"
#include "Vec.h"
double clamp(double n, double minValue, double maxValue);
string padInt(int nIn);
vector<string> split(string &inputString, char splitChar);
Scene readObj(string filename);
void writePPM(Screen &screenIn, string filename);