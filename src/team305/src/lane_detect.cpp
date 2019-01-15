#include "lane_detect.h"

Point LaneDetect::null = Point();

void LaneDetect::Setting()
{
	namedWindow("Setting", CV_WINDOW_AUTOSIZE);
	cvCreateTrackbar("Blur (2n+1)", "Setting", &BlurValue, 2);
	cvCreateTrackbar("Accuracy", "Setting", &Accuracy, 100);
}

Mat LaneDetect::ReduceNoise(const Mat &src)
{
	//imshow("Camera", src);
	Mat des(src, Rect(0, SkyLine, src.cols, src.rows - SkyLine));
	src.copyTo(draw);
	cvtColor(des, des, COLOR_RGB2GRAY);
	//GaussianBlur(des, des, Size(BlurValue << 1|1, BlurValue << 1|1), 0, 0, BORDER_DEFAULT);
	return des;
}

Mat LaneDetect::CvtBinary(const Mat &src)
{
	Mat des;

	Laplacian(src, des, CV_32F, KernelSize);
	convertScaleAbs(des, des);
	threshold(des, des, 50, 255, THRESH_BINARY);
	//morphological opening (removes small objects from the foreground)
	//erode(des, des, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)));
	//dilate(des, des, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)));
	//morphological closing (removes small holes from the foreground)
	//dilate(des, des, getStructuringElement(MORPH_RECT, Size(2, 2)));
	//erode(des, des, getStructuringElement(MORPH_RECT, Size(2, 2)));
	//imshow("Binary", des);
	return des;
}

void LaneDetect::DetectReverse(const Mat &BinarySrc)
{
	LaneL.clear();
	LaneR.clear();
	LaneM.clear();
	int saveL = 0;
	int saveR = 320;
	int S, F;

	int MidLane = BinarySrc.cols >> 1; // MidLane = car.location.x

	start = BinarySrc.rows - Box.height;
	for (int high = start; high > 20; high = high - Box.height) // with a line
	{
		// Left lane
		F = MidLane - IgnoreFromMid(high) + 20;
		if (saveL > 35)
		{
			S = saveL - 30;
		}
		else
		{
			S = 5;
		}
		for (int i = S; i < F; i = i + (Box.width >> 1))
		{
			Mat box = Mat(BinarySrc, Rect(i, high, Box.width, Box.height));
			if (countNonZero(box) * 100 < Box.area() * Accuracy)
			{
				LaneL.push_back(Point(i + 50 , high));
				saveL = i;
				break;
			}
			saveL = 0;
		}
		// Right lane
		F = MidLane + IgnoreFromMid(high) - 20;
		if (saveR < 305)
		{
			S = saveR + 10;
		}
		else
		{
			S = 315;
		}
		for (int i = S; i > F; i = i - (Box.width >> 1))
		{
			Mat box = Mat(BinarySrc, Rect(i, high, Box.width, Box.height));
			if (countNonZero(box) * 100 < Box.area() * Accuracy)
			{
				LaneR.push_back(Point(i, high));
				saveR = i;
				break;
			}
		}
	}
}

void LaneDetect::FindLane(const Mat &BinarySrc)
{
	LaneL.clear();
	LaneR.clear();
	LaneM.clear();
	int saveL = 0;
	int saveR = 0;

	int MidLane = BinarySrc.cols >> 1; // MidLane = car.location.x

	start = BinarySrc.rows - Box.height;
	for (int high = start; high > 20; high = high - Box.height) // with a line
	{
		// Left lane
		for (int i = MidLane - IgnoreFromMid(high); i > Box.width; i = i - (Box.width >> 1))
		{
			Mat box = Mat(BinarySrc, Rect(i, high, Box.width, Box.height));
			if (countNonZero(box) * 100 > Box.area() * Accuracy)
			{
				LaneL.push_back(Point(i + 30, high));
				saveL = i;
				break;
			}
			saveL = 0;
		}
		// Right lane
		for (int i = MidLane + IgnoreFromMid(high); i < BinarySrc.cols - Box.width; i = i + (Box.width >> 1))
		{
			Mat box = Mat(BinarySrc, Rect(i, high, Box.width, Box.height));
			if (countNonZero(box) * 100 > Box.area() * Accuracy)
			{
				LaneR.push_back(Point(i, high));
				saveR = i;
				break;
			}
			saveR = 0;
		}
		// Update new mid lane
		if (saveL * saveR > 0)
		{
			MidLane = MidLane * alpha + ((saveL + saveR) >> 1) * (1 - alpha);
			continue;
		}
		if (saveL + saveR == 0)
			continue;
		if (saveL == 0)
			saveL = MidLane - IgnoreFromMid(high) * 2;
		if (saveR == 0)
			saveR = MidLane + IgnoreFromMid(high) * 2;
		MidLane = MidLane * alpha + ((saveL + saveR) >> 1) * (1 - alpha);
	}
}

void LaneDetect::Detect(const Mat &src)
{
	FindLane(CvtBinary(ReduceNoise(src)));
}

void LaneDetect::DrawLane()
{
	for (int i = 0; i < LaneL.size(); i++)
		circle(draw, Point(LaneL[i].x - 30, LaneL[i].y + SkyLine), 2, Scalar(255, 0, 0), 2, 8, 0);
	for (int i = 0; i < LaneR.size(); i++)
		circle(draw, Point(LaneR[i].x, LaneR[i].y + SkyLine), 2, Scalar(0, 255, 0), 2, 8, 0);
	//imshow("Binary", draw);
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

void LaneDetect::DrawVirtualLane()
{
	LineFunction F;

	if (TrafficSign::Sign == 1)
	{

		//if (LaneR.size() < TooFew)
		//{
		//	return;
		//}
		double LocX = 0;
		double deltaX = Box.width * CarControl::TurnStrong;
		if (LaneL.size() < TooFew || F.Angle(ObjectDetect::laneL) < 30 || LaneR.size() < TooFew)
		{
			LaneL.clear();
			for (int i = start; i > 20; i = i - Box.height)
			{
				LaneL.push_back(Point(LocX, i));
				LocX += deltaX;
			}
			ObjectDetect::laneL = F.CaculateLine(LaneL[0], LaneL[LaneL.size() - 1]);
			return;
		}
	}

	if (TrafficSign::Sign == -1)
	{
		//if (LaneL.size() < TooFew)
		//{
		//	return;
		//}
		double LocX = 320;
		double deltaX = Box.width * CarControl::TurnStrong;
		if (LaneR.size() < TooFew || F.Angle(ObjectDetect::laneR) > -30 || LaneL.size() < TooFew)
		{
			LaneR.clear();
			for (int i = start; i > 20; i = i - Box.height)
			{
				LaneR.push_back(Point(LocX, i));
				LocX -= deltaX;
			}
			ObjectDetect::laneR = F.CaculateLine(LaneR[0], LaneR[LaneR.size() - 1]);
			return;
		}
	}
	
	/*
	LineFunction F;
	double TurnStrong = 2.0;
	int size = 5;

	if (TrafficSign::Sign == 1)
	{
		LaneL.clear();
		if (LaneR.size() > TooFew)
			return;
		double LocX = 0;
		double deltaX = size * TurnStrong;
		for (int i = start; i > 20; i = i - size)
		{
			LaneL.push_back(Point(LocX, i));
			LocX += deltaX;
		}
		//LanelaneL = F.CaculateLine(LaneL[0], LaneL[LaneL.size() - 1]);
		return;
	}

	if (TrafficSign::Sign == -1)
	{
		LaneR.clear();
		if (LaneL.size() > TooFew)
		{
			return;
		}
		double LocX = 320;
		double deltaX = size * TurnStrong;

		LaneR.clear();
		for (int i = start; i > 20; i = i - size)
		{
			LaneR.push_back(Point(LocX, i));
			LocX -= deltaX;
		}
		//laneR = F.CaculateLine(LaneR[0], LaneR[LaneR.size() - 1]);
		return;
	}
	*/
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
	return int(LaneDetect::Fx_a * h + LaneDetect::Fx_b);
}