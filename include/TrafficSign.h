#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <iostream>

using namespace cv;
using namespace std;

class TrafficSign
{
private:
	static bool mirror;
	static bool DetectRightOnly;
	static bool ShowCam;

	int iIgnoreObj;
	int iSizeF;

	int iLowH, iHighH;
	int iLowS, iHighS;
	int iLowV, iHighV;

	Mat img;

	void PreFixImg();
	int DetectedContours(Mat &src);
public:
	TrafficSign();
	TrafficSign(const Mat &src);
	~TrafficSign() {};
	void InitControl();
	int IsTrafficSigns();
};