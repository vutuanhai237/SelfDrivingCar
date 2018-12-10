#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv/cv.h>
#include <iostream>

using namespace cv;
using namespace std;

class LaneDetect
{
#pragma region Setting value
	static const int SkyLine; // =80
	static const Size Box; // 5x5 - Box detetion lane
	static const int IgnoreFromMid; //50 - distance form  mid will be ignore from lane detect
	static const float alpha;

	static int BlurValue;
	static int KernelSize;

	static int Accuracy;
#pragma endregion
public:
#pragma region Lane
	static vector<Point> LaneL;
	static vector<Point> LaneR;
	static Mat draw;
#pragma endregion
	void Setting();
	Mat ReduceNoise(const Mat &src);
	Mat CvtBinary(const Mat &src);
	void FindLane(const Mat &BinarySrc);
	void DrawLane();
	~LaneDetect();
};