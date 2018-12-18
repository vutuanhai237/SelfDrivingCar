

#include "LaneDetect.h"

void LaneDetect::Setting()
{
	namedWindow("Setting", CV_WINDOW_AUTOSIZE);
	cvCreateTrackbar("Blur", "Setting", &BlurValue, 10);
	cvCreateTrackbar("Accuracy", "Setting", &Accuracy, 100);
}

Mat LaneDetect::ReduceNoise(const Mat &src)
{
	imshow("Camera", src);
	Mat des(src, Rect(0, SkyLine, src.cols, src.rows - SkyLine));
	des.copyTo(draw);
	//draw.release();
	rectangle(draw, Rect(0, 0, draw.cols, draw.rows), Scalar(0, 0, 0), -1);
	
	cvtColor(des, des, COLOR_RGB2GRAY);
	GaussianBlur(src, src, Size(BlurValue, BlurValue), 0, 0, BORDER_DEFAULT);
	return des;
}

Mat LaneDetect::CvtBinary(const Mat &src)
{
	Mat des;
	Laplacian(src, des, CV_16S, KernelSize);
	convertScaleAbs(des, des);
	threshold(des, des, 50, 255, THRESH_BINARY);
	//morphological opening (removes small objects from the foreground)
	erode(des, des, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)));
	dilate(des, des, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)));
	//morphological closing (removes small holes from the foreground)
	dilate(des, des, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	erode(des, des, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	imshow("Binary", des);
	return des;
}

void LaneDetect::FindLane(const Mat &BinarySrc)
{
	LaneL.clear();
	LaneR.clear();
	LaneM.clear();
	int saveL = 0;
	int saveR = 0;

	int MidLane = BinarySrc.cols >> 1; // MidLane = car.location.x

	for (int high = BinarySrc.rows - Box.height; high > 20; high = high - Box.height) // with a line
	{
		// Left lane
		for (int i = MidLane - IgnoreFromMid(high); i > Box.width; i = i - (Box.width >> 1))
		{
			Mat box = Mat(BinarySrc, Rect(i, high, Box.width, Box.height));
			if (countNonZero(box) * 100 > Box.area()*Accuracy)
			{
				LaneL.push_back(Point(i, high));
				saveL = i;
				break;
			}
		}
		// Right lane
		for (int i = MidLane + IgnoreFromMid(high); i < BinarySrc.cols - Box.width; i = i + (Box.width >> 1))
		{
			Mat box = Mat(BinarySrc, Rect(i, high, Box.width, Box.height));
			if (countNonZero(box) * 100 > Box.area()*Accuracy)
			{
				LaneR.push_back(Point(i, high));
				saveR = i;
				break;
			}
		}
		// Update new mid lane
		if (saveL*saveR > 0)
		{
			MidLane = MidLane * alpha + ((saveL + saveR) >> 1)*(1 - alpha);
		}
	}
}

void LaneDetect::Detect(const Mat & src)
{
	FindLane(CvtBinary(ReduceNoise(src)));
}

void LaneDetect::DrawLane()
{
	for (int i = 0; i < LaneL.size(); i++)
		circle(draw, LaneL[i], 2, Scalar(255, 0, 0), 2,8,0);
	for (int i = 0; i < LaneR.size(); i++)
		circle(draw, LaneR[i], 2, Scalar(0, 255, 0), 2, 8, 0);
	//for (int i = 0; i < LaneM.size(); i++)
	//	circle(draw, LaneM[i], 2, Scalar(255, 255, 255), 2, 8, 0);
	imshow("Lane detect", draw);
}

void LaneDetect::UpdateMidLane()
{
	int i = 0;
	int j = 0;
	while (i < LaneL.size() && j < LaneR.size())
	{
		while (i < LaneL.size() && LaneL[i].y > LaneR[j].y)
			i++;
		if (i == LaneL.size())
			return;
		while (j < LaneR.size() && LaneR[j].y > LaneL[i].y)
			j++;
		if (j == LaneR.size())
			return;
		if (LaneL[i].y == LaneR[j].y)
		{
			LaneM.push_back(Point((LaneL[i].x + LaneR[j].x) >> 1, LaneL[i].y));
			i++;
			j++;
		}
	}
}

LaneDetect::~LaneDetect()
{
	if (!LaneL.empty())
		LaneL.clear();
	if (!LaneR.empty())
		LaneR.clear();
}

// Other function
int LaneDetect::IgnoreFromMid(int h)
{
	return int(LaneDetect::Fx_a*h + LaneDetect::Fx_b);
}