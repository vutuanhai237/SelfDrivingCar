#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace cv;

class TrafficSign
{
#pragma region static var
	static int iHighH;
	static int iLowH;
	static int iHighL;
	static int iLowL;
	static int iHighS;
	static int iLowS;
	static int CountFrame;
	static const int MinSquare = 200; // 200 pixel - That is a size that use for 
	static const int LockFlag = 10;// 10 frame - after 10 frame detect a same sign, lock that result and stop detect
	static const int UnlockFlag = 50; // ~2s
	static bool flag;
	static int OldSign;
#pragma endregion
public:
	void Setting();
	int Find(Mat &src);
private:
	Mat PreFix(const Mat &src); // return a matrix Threshold for detection
	Mat ThresholdDetection(Mat &draw, const Mat &Thres); // return a matrix what is most likely a traffic sign
	int CheckSign(const Mat &src); // return 0 if none sign, -1 if turn left, 1 if turn right
};
