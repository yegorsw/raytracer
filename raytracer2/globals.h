#pragma once

//#define DEBUG_OUTPUTALPHA
//#define DEBUG_OUTPUTDIST
//#define DEBUG_OUTPUTPOS
//#define DEBUG_OUTPUTNORMALS
//#define DEBUG_OUTPUTSAMPLES
//#define DEBUG_OUTPUTINTERSECTIONS

const string objfile = "refrtest";
const string rootdir = "C:/Users/Yegor-s/Desktop/raytracer/";

const double SCN_MAXDIST = 1000000000;

const int IMG_W = 1000;
const int IMG_H = 1000;

const double SCN_RAYBIAS = 0.00001;
const int SCN_MAXDEPTH = 50;

const int SHD_MAXSAMPLES = 8;

const int SAMP_MINSAMPLES = 1;
const int SAMP_MAXSAMPLES = 1;
const double SAMP_MAXVARIANCE = 0.001;

const int NUM_THREADS = 16;