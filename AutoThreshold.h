#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

using namespace cv;
using namespace std;

void Setting();
void AutoThreshold(const Mat &src);