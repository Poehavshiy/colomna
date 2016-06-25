#ifndef MYfunctions_h
#define MYfunctions_h

#include <iostream>
#include <fstream> 
#include "opencv2/opencv.hpp"
#include <stdio.h>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
//#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/nonfree/nonfree.hpp"
#include <iterator>

using namespace cv;
using namespace std;

extern int iLowH;
extern int iHighH;
extern int iLowS;
extern int iHighS;
extern int iLowV;
extern int iHighV;
extern int CATCH;
extern int MAP;

double FindMaks(Mat *A);

double FindAverage(Mat *A);

std::string getImageType(int number);

int NumnonZEROPIX(Mat *A);

void histogr(Mat *src, Mat *dst);

void ColourFilt(Mat *src, Mat *dst);

void TrackBar(int &iLowH, int &iHighH, int &iLowS, int &iHighS, int &iLowV, int &iHighV);

void TrackBarBool(int &CATCH, int &MAP);

Point contors(Mat *Filtered);

Point2f *contorsForCalib(Mat Filtered, int Number);

void check(Mat A, Point2f *inputQuad, int Number);

void Create_windows();

Mat getMfrFILE(const char *adress);

Point2f MatixVectorMult(const Mat *oper, Point2f P);

void writeinf(FILE *f, Mat A);

void DirtySort(Point2f *outputQuad, int Number);

#endif