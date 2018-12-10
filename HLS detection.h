#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace cv;

class HLSdetetion
{
#pragma region static var
	static int iHighH;
	static int iLowH;
	static int iHighL;
	static int iLowL;
	static int iHighS;
	static int iLowS;
	static int CountFrame;
	static const int MinSquare = 200;
	static const int LockFlag = 10; // 10 frame
	static const int UnlockFlag = 50; // ~2s
	static bool flag;
	static int OldSign;
#pragma endregion
public:
	void Setting();
	Mat PreFix(const Mat &src); // return a matrix Threshold for detection
	Mat ThresholdDetection(Mat &draw, const Mat &Thres); // return a matrix what is most likely a traffic sign
	int CheckSign(const Mat &src); // return 0 if none sign, -1 if turn left, 1 if turn right
};
