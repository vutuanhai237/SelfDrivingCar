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
private:
	static const int SkyLine; // =80
	static const Size Box; // 5x5 - Box detetion lane
	static const float alpha;

	static int BlurValue;
	static int KernelSize;

	static int Accuracy;
public:
	static double Fx_a;
	static double Fx_b;
#pragma endregion
#pragma region Lane
public:
	static vector<Point> LaneL;
	static vector<Point> LaneR;
	static vector<Point> LaneM;
	static Mat draw;
#pragma endregion
public:
	void Setting();
	void Detect(const Mat &src);
	void DrawLane();
	~LaneDetect();
private:
	Mat ReduceNoise(const Mat &src);
	Mat CvtBinary(const Mat &src);
	void FindLane(const Mat &BinarySrc);
	int IgnoreFromMid(int h);
};