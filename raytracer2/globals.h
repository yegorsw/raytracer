#pragma once

//#define DEBUG_OUTPUTALPHA
//#define DEBUG_OUTPUTDIST
//#define DEBUG_OUTPUTPOS
//#define DEBUG_OUTPUTNORMALS
//#define DEBUG_OUTPUTSAMPLES
//#define DEBUG_OUTPUTINTERSECTIONS

const double SCN_MAXDIST = 5000;

const int IMG_W = 1000;
const int IMG_H = 500;

const double SCN_RAYBIAS = 0.001;
const int SCN_MAXDEPTH = 1;

const int SHD_MAXSAMPLES = 32;

const int SAMP_MINSAMPLES = 24;
const int SAMP_MAXSAMPLES = 1024;
const double SAMP_MAXVARIANCE = 0.005;

const int NUM_THREADS = 1;