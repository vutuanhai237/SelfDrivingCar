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

	VideoCapture camera("test cua2.mp4");
	while (true)
	{
		Mat img;
		camera >> img;
		if (img.empty())
			break;
		lane.Detect(img);
		double angle = car.GetAngle();
		cout << sign.Find(img) << "\t\t" << angle << "\t\t" << car.GetSpeed(angle) << endl;
		imshow("Lane detect", LaneDetect::draw);
		if (waitKey(30) == 27)
			break;
	}
}