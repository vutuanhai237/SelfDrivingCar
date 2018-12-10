#include "TrafficSign.h"

void TrafficSign::PreFixImg()
{
	if (DetectRightOnly)
		img = Mat(img, Rect(img.cols >> 1, 0, img.cols >> 1, img.rows)); // Lay lan duong ben phai

	cvtColor(img, img, COLOR_BGR2HSV, 0);

	inRange(img, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), img);
	//morphological opening (removes small objects from the foreground)
	erode(img, img, getStructuringElement(MORPH_ELLIPSE, Size(iIgnoreObj, iIgnoreObj)));
	dilate(img, img, getStructuringElement(MORPH_ELLIPSE, Size(iIgnoreObj, iIgnoreObj)));
	//morphological closing (removes small holes from the foreground)
	dilate(img, img, getStructuringElement(MORPH_ELLIPSE, Size(iIgnoreObj, iIgnoreObj)));
	erode(img, img, getStructuringElement(MORPH_ELLIPSE, Size(iIgnoreObj, iIgnoreObj)));
}

int TrafficSign::DetectedContours(Mat &src)
{
	RNG rng(12345);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	// Zoning identification
	img.copyTo(src);
	findContours(img, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	Rect rec = boundingRect(contours[0]);
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		// Find bigest obj
		Rect tmp = boundingRect(contours[i]);
		if (tmp.area() > rec.area())
			rec = tmp;
	}
	// ignore maybe none obj
	// Draw bigest obj
	if (DetectRightOnly)
		rectangle(src, Rect(rec.x + src.cols, rec.y, rec.width, rec.height), Scalar(255, 0, 0), 2);
	else
		rectangle(src, Rect(rec.x, rec.y, rec.width, rec.height), Scalar(0, 0, 255), 2);

	// Fix ellipse to circle
	Size transf(iSizeF, iSizeF);
	Mat fix;
	resize(img, fix, transf);
	resize(Mat(img, rec), fix, transf);

	if (ShowCam)
		imshow("fix to circle", fix);

	// Is Circle
	vector<Vec3f> circles;
	HoughCircles(fix, circles, CV_HOUGH_GRADIENT, 2, 5);
	if (circles.size() <= 0)
		return 0;
	Mat HalfL = Mat(fix, Rect(0, 0, iSizeF >> 1, iSizeF >> 1));
	Mat HalfR = Mat(fix, Rect(iSizeF >> 1, 0, iSizeF >> 1, iSizeF >> 1));

	if (ShowCam)
	{
		imshow("HalfL", HalfL);
		imshow("HalfR", HalfR);
	}

	if (countNonZero(HalfL) < countNonZero(HalfR))
		return -1;
	else
		return 1;
}

TrafficSign::TrafficSign()
{
	iIgnoreObj = 1;
	iSizeF = 50;

	iLowH = 75;
	iHighH = 130;

	iLowS = 100;
	iHighS = 255;

	iLowV = 100;
	iHighV = 255;
}

TrafficSign::TrafficSign(Mat src)
{
	iIgnoreObj = 1;
	iSizeF = 50;

	iLowH = 75;
	iHighH = 130;

	iLowS = 100;
	iHighS = 255;

	iLowV = 100;
	iHighV = 255;
	img = src;
}

void TrafficSign::InitControl()
{
	namedWindow("Control", CV_WINDOW_AUTOSIZE);

	cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
	cvCreateTrackbar("HighH", "Control", &iHighH, 179);

	cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	cvCreateTrackbar("HighS", "Control", &iHighS, 255);

	cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
	cvCreateTrackbar("HighV", "Control", &iHighV, 255);
}

int TrafficSign::IsTrafficSigns()
{
	if (mirror)
		flip(img, img, 1);
	Mat src;
	img.copyTo(src);
	PreFixImg();
	int res = DetectedContours(src);
	if (ShowCam)
	{
		imshow("real", src);
		imshow("fix", img);
	}
	return res;
}
