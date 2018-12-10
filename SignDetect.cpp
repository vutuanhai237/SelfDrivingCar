#include "SignDetect.h"

void PreFixImg(Mat &src, Mat &des)
{
	if (DetectRightOnly)
		src = Mat(src, Rect(src.cols >> 1, 0, src.cols >> 1, src.rows)); // Lay lan duong ben phai

	cvtColor(src, des, COLOR_BGR2HSV, 0);

	inRange(des, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), des);
	//morphological opening (removes small objects from the foreground)
	erode(des, des, getStructuringElement(MORPH_ELLIPSE, Size(iIgnoreObj, iIgnoreObj)));
	dilate(des, des, getStructuringElement(MORPH_ELLIPSE, Size(iIgnoreObj, iIgnoreObj)));
	//morphological closing (removes small holes from the foreground)
	dilate(des, des, getStructuringElement(MORPH_ELLIPSE, Size(iIgnoreObj, iIgnoreObj)));
	erode(des, des, getStructuringElement(MORPH_ELLIPSE, Size(iIgnoreObj, iIgnoreObj)));
}

int DetectedContours(Mat &src, Mat &des)
{
	RNG rng(12345);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	// Zoning identification
	
	findContours(des, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	Rect rec(0, 0, 0, 0);
	bool FirstTime = true;

	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		// Find bigest obj
		Rect tmp = boundingRect(contours[i]);
		if ((tmp.width > (tmp.height << 2)) || ((tmp.width << 2) < tmp.height))
			continue;
		if (tmp.area() > Square)
			if (FirstTime)
				rec = tmp;
			else
				if (rec.area() < tmp.area())
					rec = tmp;
	}
	if (rec.area() < Square)
		return 0;
	rectangle(src, Rect(rec.x, rec.y, rec.width, rec.height), Scalar(0, 0, 255), 2);
	// forcus to obj detected and cover to circle
	Size transf(iSizeF, iSizeF);
	Mat fix(des, rec);
	resize(fix, fix, transf);

	if (ShowCam)
		imshow("fix to circle", fix);
	// is circle ?

	vector<Vec3f> circles;
	HoughCircles(fix, circles, CV_HOUGH_GRADIENT, 2, 5);
	if (circles.size() <= 0)
		return 0;

	// left or right ?
	Mat HalfL(fix, Rect(0, 0, rec.width >> 1, rec.height >> 1));
	Mat HalfR(fix, Rect(rec.width >> 1, 0, rec.width >> 1, rec.height >> 1));

	Mat out();
	//if (ShowCam)
	//{
	//	imshow("HalfL", HalfL);
	//	imshow("HalfR", HalfR);
	//}

	if (countNonZero(HalfL) < countNonZero(HalfR))
		return -1;
	else
		return 1;
	return 0;
}

int IsTrafficSigns(const Mat &src)
{
	Mat cut, res;
	src.copyTo(cut);
	PreFixImg(cut, res);
	int tmp = DetectedContours(cut, res);
	if (ShowCam)
	{
		imshow("video", src);
		imshow("cut", cut);
		imshow("binary", res);
	}
	return tmp;
}
