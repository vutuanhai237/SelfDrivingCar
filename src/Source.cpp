#include "Setting.h"
#include "TrafficSign.h"
#include "LaneDetect.h"
#include "CarControl.h"

int main()
{
	TrafficSign sign;
	LaneDetect lane;
	CarControl car;
	lane.Setting();

	VideoCapture camera("test.mp4");
	while (true)
	{
		Mat img;
		camera >> img;
		if (img.empty())
			break;
		lane.Detect(img);
		lane.DrawLane();
		cout << sign.Find(img) << "\t\t" << car.GetAngle(LaneDetect::LaneL, LaneDetect::LaneR, LaneDetect::LaneM)<< endl;

		if (waitKey(30) == 27)
			break;
	}
}