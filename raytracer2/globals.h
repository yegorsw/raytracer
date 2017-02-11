#pragma once

//#define DEBUG_OUTPUTALPHA
//#define DEBUG_OUTPUTDIST
//#define DEBUG_OUTPUTPOS
//#define DEBUG_OUTPUTNORMALS
//#define DEBUG_OUTPUTSAMPLES
//#define DEBUG_OUTPUTINTERSECTIONS

const string objfile = "angelshiny";
const string rootdir = "C:/Users/Yegor-s/Desktop/raytracer/";

const double SCN_MAXDIST = 1000000000;

const int IMG_W = 600;
const int IMG_H = 600;

const double SCN_RAYBIAS = 0.001;
const int SCN_MAXDEPTH = 2;

const int SHD_MAXSAMPLES = 8;

const int SAMP_MINSAMPLES = 4;
const int SAMP_MAXSAMPLES = 512;
const double SAMP_MAXVARIANCE = 0.001;

const int NUM_THREADS = 16;