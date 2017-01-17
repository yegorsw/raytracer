#pragma once

//#define DEBUG_OUTPUTALPHA
//#define DEBUG_OUTPUTDIST
//#define DEBUG_OUTPUTPOS
//#define DEBUG_OUTPUTNORMALS
//#define DEBUG_OUTPUTSAMPLES
//#define DEBUG_OUTPUTINTERSECTIONS

const string objfile = "simpleboxesshaded";
const string rootdir = "D:/Users/Yegor/Desktop/raytracer/";

const double SCN_MAXDIST = 1000000000;

const int IMG_W = 600;
const int IMG_H = 600;

const double SCN_RAYBIAS = 0.001;
const int SCN_MAXDEPTH = 3;

const int SHD_MAXSAMPLES = 8;

const int SAMP_MINSAMPLES = 16;
const int SAMP_MAXSAMPLES = 256;
const double SAMP_MAXVARIANCE = 0.01;

const int NUM_THREADS = 16;