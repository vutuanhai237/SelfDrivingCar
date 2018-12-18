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

	VideoCapture camera("test cua.mp4");
	while (true)
	{
		Mat img;
		camera >> img;
		if (img.empty())
			break;
		lane.Detect(img);
		//lane.DrawLane();
		double angle = car.GetAngle();
		cout << sign.Find(img) << "\t\t" << angle << "\t\t" << car.GetSpeed(angle) << endl;
		lane.DrawLane();
		imshow("Lane detect", LaneDetect::draw);
		if (waitKey(30) == 27)
			break;
	}
}