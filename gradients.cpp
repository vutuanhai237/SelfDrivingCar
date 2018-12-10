#include "Gradients.h"
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

static int BlurValue = 5;
static int KernelSize = 3;

static int Noise = 50; //50 
static int percent = 55; // 55%
#pragma region Flag
static bool LostL = false;
static bool LostR = false;
static bool Object = false;
#pragma endregion
#pragma region const var
static const double PI_ = acos(-1.0); // PI~3.14159
static const int SkyLine = 80;  // 80 = 1/3 rows
static const Size resolution(320, 240 - SkyLine); // 320x160 = 320x(240-Skyline)
static const int DetectionLine = 50; //50 - Line use for lane detetion
static const Size BoxApply(5, 5); // 5x5 - Box detetion lane
static const float alpha = 0.15; // 0.15 - Value reduce noise
static const int ignoreFromMid = 30; // 30 - Min distance from mid-road to side-road
static const int limit = 10; // 10 - Limit noise
static const int SpeedLimit = 70;
static const int SpeedMin = 20;
static const int CarSize = 80;
#pragma endregion
static Point OldL(0, 0);
static Point OldR(0, 0);
static float OldA = 0;
//static int OldS = 0;

int GetSpeed(const float & angle)
{
	int speed = SpeedLimit - abs(angle)*2;
	return speed > SpeedMin ? speed : SpeedMin;
}

float GetAngle(const Point2f & CarLoction)
{
	if (LostL && LostR)
		return OldA;
	float dx = MidRoad - CarLoction.x;
	if (LostL)
		dx = OldR.x - (CarSize >> 1) - CarLoction.x;
	if (LostR)
		dx = OldL.x + (CarSize >> 1) - CarLoction.x;
	float dy = resolution.height - DetectionLine;
	OldA = atan(dx / dy) * 180 / PI_;
	return OldA;
}

int UpdateMidRoad(const int &mid)
{
	//if (LostL)
	//	MidRoad = OldR.x - CarSize;
	//if (LostR)
	//	MidRoad = OldL.x + CarSize;
	if (LostL || LostR)
		return MidRoad;
	//return mid*(1 - alpha) + MidRoad*alpha;
	return mid * alpha + (1 - alpha)*MidRoad;
}

void UpdateFlag(const Point &L, const Point &R)
{
	if ((abs(L.x - OldL.x) > limit) && (abs(MidRoad - (RoadSize >> 1) - L.x) > limit))
		LostL = true;
	if ((abs(R.x - OldR.x) > limit) && (abs(MidRoad + (RoadSize >> 1) - R.x) > limit))
		LostR = true;
	if (!LostL)
		OldL.x = L.x;
	if (!LostR)
		OldR.x = R.x;
	if (LostL && LostR)
	{
		OldL.x = L.x;
		OldR.x = R.x;
	}
}

int UpdateRoadSize(const Point &L, const Point &R)
{
	if (LostL || LostR)
		return RoadSize;
	return R.x - L.x;
}

void GradientsSetting()
{
	namedWindow("Setting", CV_WINDOW_AUTOSIZE);
	cvCreateTrackbar("Blur", "Setting", &BlurValue, 10);
	cvCreateTrackbar("Noise", "Setting", &Noise, 255);
	cvCreateTrackbar("Accuracy", "Setting", &percent, 100);
}

void GradientsDetect(const Mat &src)
{
	try
	{
		LostL = true;
		LostR = true;

		Mat saved;
		src.copyTo(saved);
		//imshow("src", src);
		Mat crop;
		crop = Mat(src, Rect(0, SkyLine, src.cols, src.rows - SkyLine));
		
		Mat gray;
		crop.copyTo(gray);
		
		cvtColor(gray, gray, COLOR_RGB2GRAY);
		//imshow("gray", gray);
		
		/* Binary */
		Mat blur;
		gray.copyTo(blur);
		if (BlurValue > 0)
			GaussianBlur(src, src, Size(BlurValue, BlurValue), 0, 0, BORDER_DEFAULT);
		//imshow("blur", blur);

		Mat Lap;
		Laplacian(blur, Lap, CV_16S, KernelSize);
		//Laplacian(blur, Lap, CV_16S, KernelSize,1.5);
		convertScaleAbs(Lap, Lap);
		
		Mat test19;
		Lap.copyTo(test19);
		
		//imshow("Final", Lap);

		// Lane detection
		Point LaneL(0, SkyLine + DetectionLine), LaneR(0, SkyLine + DetectionLine);
		Mat control, MatBox;
		resize(Lap, control, resolution);
		threshold(control, control, 50, 255, THRESH_BINARY);
		//GaussianBlur(control, control, Size(3, 3), 0);
		imshow("control", control);
		for (int i = MidRoad - BoxApply.width - ignoreFromMid; i > BoxApply.width; i--)
		{
			LaneL.x = i;
			MatBox = Mat(control, Rect(i - (BoxApply.width >> 1), DetectionLine - (BoxApply.height >> 1), BoxApply.width, BoxApply.height));
			if (countNonZero(MatBox) * 100 > percent*BoxApply.area())
			{
				rectangle(saved, Rect(i - (BoxApply.width >> 1), DetectionLine - (BoxApply.height >> 1) + SkyLine, 2, BoxApply.height << 1), Scalar(0, 0, 255));
				LostL = false;
				break;
			}
		}

		for (int i = MidRoad + BoxApply.width + ignoreFromMid; i < resolution.width - (BoxApply.width >> 1); i++)
		{
			LaneR.x = i;
			MatBox = Mat(control, Rect(i - (BoxApply.width >> 1), DetectionLine - (BoxApply.height >> 1), BoxApply.width, BoxApply.height));
			if (countNonZero(MatBox)*100 > percent*BoxApply.area())
			{
				rectangle(saved, Rect(i - (BoxApply.width >> 1), DetectionLine - (BoxApply.height >> 1) + SkyLine, 2, BoxApply.height << 1), Scalar(0, 0, 255)); 
				LostR = false;
				break;
			}
		}
		// car control
		UpdateFlag(LaneL, LaneR);
		RoadSize = UpdateRoadSize(LaneL, LaneR);
		MidRoad = UpdateMidRoad(LaneL.x + LaneR.x >> 1);
		rectangle(saved, Rect(MidRoad, LaneL.y - 10, 2, 20), Scalar(0, 255, 0));
		
		string tmp = to_string(GetAngle(Point2f(resolution.width >> 1, resolution.height)));

		//putText(saved, tmp, Point(10, 50), FONT_HERSHEY_COMPLEX_SMALL, 2, Scalar(0, 0, 0),3);

		cout << GetAngle(Point2f(resolution.width >> 1, resolution.height)) << setprecision(4) << "\t\t" << GetSpeed(GetAngle(Point2f(resolution.width >> 1, resolution.height))) << "\t\t" << RoadSize << "\t\t";
		imshow("Lane detection", saved);
	}
	catch(exception){}
}


