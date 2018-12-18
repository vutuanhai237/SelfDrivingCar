#include "traffic_sign.h"
#include <iostream>

using namespace std;

void TrafficSign::Setting()
{
	namedWindow("Setting", CV_WINDOW_AUTOSIZE);
	cvCreateTrackbar("LowH", "Setting", &iLowH, 255);
	cvCreateTrackbar("HighH", "Setting", &iHighH, 255);
	cvCreateTrackbar("LowL", "Setting", &iLowL, 255);
	cvCreateTrackbar("HighL", "Setting", &iHighL, 255);
	cvCreateTrackbar("LowS", "Setting", &iLowS, 255);
	cvCreateTrackbar("HighS", "Setting", &iHighS, 255);
}

int TrafficSign::Find(Mat & src)
{
	return CheckSign(ThresholdDetection(src, PreFix(src)));
}

Mat TrafficSign::PreFix(const Mat &src)
{
	//imshow("Camera", src);
	int iIgnoreObj = 1;
	//Mat des(src, Rect(src.cols >> 0, 0, src.cols >> 0, src.rows));
	//imshow("Real", des);
	//cvtColor(des, des, COLOR_RGB2HLS);
	Mat des(src);
	cvtColor(des, des, COLOR_BGR2HLS);

	imshow("HLS", des);

	GaussianBlur(des, des, Size(3, 3), 0);
	inRange(des, Scalar(iLowH, iLowL, iLowS), Scalar(iHighH, iHighL, iHighS), des);
	//morphological opening (removes small objects from the foreground)
	erode(des, des, getStructuringElement(MORPH_ELLIPSE, Size(iIgnoreObj, iIgnoreObj)));
	dilate(des, des, getStructuringElement(MORPH_ELLIPSE, Size(iIgnoreObj, iIgnoreObj)));
	//morphological closing (removes small holes from the foreground)
	dilate(des, des, getStructuringElement(MORPH_ELLIPSE, Size(iIgnoreObj, iIgnoreObj)));
	erode(des, des, getStructuringElement(MORPH_ELLIPSE, Size(iIgnoreObj, iIgnoreObj)));

	imshow("IHLS threshold", des);

	return des;
}

// Draw rectangle theshold detetion from matrix Thres into matrix draw
Mat TrafficSign::ThresholdDetection(Mat &draw, const Mat &Thres)
{
	RNG rng(12345);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(Thres, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	Rect rec(0, 0, 1, 1);

	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		// Find bigest obj
		Rect tmp = boundingRect(contours[i]);
		if ((tmp.width > (tmp.height << 1)) || ((tmp.width << 1) < tmp.height))
			continue;
		if (tmp.area() > MinSquare)
			if (rec.area() < tmp.area())
				rec = tmp;
	}
	if (rec.area() < MinSquare)
		return Mat(draw, Rect(0, 0, 1, 1));
	rectangle(draw, Rect(rec.x, rec.y, rec.width, rec.height), Scalar(0, 0, 255));

	//imshow("Real", draw);

	// forcus to obj detected and cover to square
	Mat fix(Thres, rec);
	if (rec.height > rec.width)
		resize(fix, fix, Size(rec.height, rec.height));
	else
		resize(fix, fix, Size(rec.width, rec.width));
	//imshow("forcus", fix);

	return fix;
}

int TrafficSign::CheckSign(const Mat &src)
{
	if (flag)
	{
		if (TrafficSign::CountFrame > TrafficSign::UnlockFlag)
		{
			TrafficSign::CountFrame = 0;
			TrafficSign::flag = false;
			TrafficSign::Sign = 0;
			CarControl::MaxSpeed = 80;
		}
		else
		{
			TrafficSign::CountFrame++;
			return TrafficSign::OldSign;
		}
	}

	if (src.rows < 3)
	{
		TrafficSign::CountFrame = 0;
		return 0;
	}

	Mat HalfL(src, Rect(0, 0, src.cols >> 1, src.rows >> 1));
	Mat HalfR(src, Rect(src.cols >> 1, 0, src.cols >> 1, src.rows >> 1));

	int sign = countNonZero(HalfL) < countNonZero(HalfR) ? -1 : 1;
	if (TrafficSign::CountFrame == 0)
	{
		OldSign = sign;
		CountFrame = 1;
	}
	else
		if (sign = OldSign)
			TrafficSign::CountFrame++;
		else
		{
			OldSign = sign;
			CountFrame = 1;
		}
	if (TrafficSign::CountFrame > TrafficSign::LockFlag)
	{
		// confim traffic sign
		flag = true;
		CountFrame = 1;
		TrafficSign::Sign = sign;
		CarControl::MaxSpeed = CarControl::MinSpeed;
		return sign;
	}

	return 0; // can't confim traffic sign
}