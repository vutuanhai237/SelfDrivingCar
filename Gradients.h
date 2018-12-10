#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv/cv.h>

// khi mat lan thi cap nhat lai midroad theo lan con co va roadsize

using namespace cv;

static int MidRoad = 320 >> 1; // 160
static int RoadSize = 80; // 80

int GetSpeed(const float &angle);
float GetAngle(const Point2f &CarLoction);
void GradientsSetting();
void GradientsDetect(const Mat &src);
