#pragma once
#include "AutoThreshold.h"

static int Min_Threshold = 127;
static int Max_Threshold = 255;
static int BlurValue = 0;

void Setting()
{
	namedWindow("Setting", CV_WINDOW_AUTOSIZE);
	cvCreateTrackbar("Blur", "Setting", &BlurValue, 5);
}

void AutoThreshold(const Mat & src)
{
	imshow("Original", src);
	//
	Mat Gray;
	cvtColor(src, Gray, COLOR_RGB2GRAY);
	try
	{
		medianBlur(Gray, Gray, BlurValue);
	}
	catch(exception){};
	imshow("Gray", Gray);
	//
	Mat th1, th2;
	adaptiveThreshold(Gray, th1, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 11, 2);
	imshow("Adaptive Mean Thresholding", th1);
	//
	adaptiveThreshold(Gray, th2, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 2);
	imshow("Adaptive Gaussian Thresholding", th2);
}
