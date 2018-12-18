#include "car_control.h"

CarControl::CarControl()
{
	angle_publisher = node_obj1.advertise<std_msgs::Float32>("Team1_steerAngle", 0);
	speed_publisher = node_obj2.advertise<std_msgs::Float32>("Team1_speed", 0);
}
CarControl::~CarControl() {}
void sleep(int milliseconds)
{
	clock_t time_end;
    time_end = clock() + milliseconds * CLOCKS_PER_SEC/1000;
    while (clock() < time_end)
    {
		
    }
}

void CarControl::driverCar(Mat & out)
{
	int velocity;
	double angle_info;

	// TrafficSign *F ;
	// if (F->Find(out) == -1)
	// {
		// angle_info = -45;
	// }
	// else if (F->Find(out) == 1)
	// {
		// angle_info = 45;
	// }
	// else
	// {
	angle_info = CarControl::GetAngle();
	//}
	velocity = CarControl::GetSpeed(angle_info);
		
	//cv::imshow("sign", out);

	std_msgs::Float32 angle;
	std_msgs::Float32 speed;

	angle.data = angle_info;
	speed.data = velocity;

	angle_publisher.publish(angle);
	speed_publisher.publish(speed);
}


void CarControl::UpdateSizeLane(int NewSizeLane)
{
	SizeLane = SizeLane * (1 - Alpha) + NewSizeLane * Alpha;
}

double CarControl::Fx(int x)
{
	return FxSize_a * x + FxSize_b;
}

double CarControl::ReduceAngle(const double & angle)
{
	return (CarControl::OldAngle*0.8 + angle * 0.2)*0.8;
}

double CarControl::GetAngle()
{
	Line res;
	LineFunction F;
	LaneDetect L;
	ObjectDetect O;
	O.ClearObject();


	// Lost lane, kept old angle
	if (LaneDetect::LaneL.size() + LaneDetect::LaneR.size() < (TooFew << 1))
		return OldAngle;
	// Lost left lane, follow right lane
	if (LaneDetect::LaneL.empty())
	{
		res = ObjectDetect::laneR;

		int tmp = LaneDetect::LaneR.size() - 6;
		OldAngle = F.Angle(CarLocation, Point(LaneDetect::LaneR[tmp].x-(SizeLane>>1),LaneDetect::LaneR[tmp].y));
		line(LaneDetect::draw, CarLocation, Point(LaneDetect::LaneR[tmp].x-(SizeLane>>1),LaneDetect::LaneR[tmp].y), Scalar(0,0,255));
		
			
		//OldAngle = F.Angle(CarLocation, Point(F.ReturnX(res, LineDetect) - (SizeLane >> 2), LineDetect));
		//line(LaneDetect::draw, CarLocation, Point(F.ReturnX(res, LineDetect) - (SizeLane >> 2), LineDetect), Scalar(0, 0, 255));
		L.DrawLane();
		return OldAngle;
	}
	// Lost right lane, follow left lane
	if (LaneDetect::LaneR.empty())
	{
		res = ObjectDetect::laneL;
		//OldAngle = F.Angle(CarLocation, Point(F.ReturnX(res, LineDetect) + (SizeLane >> 1), LineDetect));
		//line(LaneDetect::draw, CarLocation, Point(F.ReturnX(res, LineDetect) + (SizeLane >> 1), LineDetect), Scalar(0, 0, 255));
		
		int tmp = LaneDetect::LaneL.size() - 6;
		OldAngle = F.Angle(CarLocation, Point(LaneDetect::LaneL[tmp].x+(SizeLane>>1),LaneDetect::LaneL[tmp].y));
		line(LaneDetect::draw, CarLocation, Point(LaneDetect::LaneL[tmp].x+(SizeLane>>1),LaneDetect::LaneL[tmp].y), Scalar(0,0,255));

		L.DrawLane();
		return OldAngle;
	}
	// no lost lane
	L.UpdateMidLane();
	res = F.Linear(LaneDetect::LaneM);
	UpdateSizeLane((F.ReturnX(ObjectDetect::laneR, LineDetect) - F.ReturnX(ObjectDetect::laneL, LineDetect)) >> 1);
	OldAngle = ReduceAngle(F.Angle(CarLocation, Point(F.ReturnX(res, LineDetect), LineDetect)));
	// draw
	line(LaneDetect::draw, Point(F.ReturnX(res, 0), 0), Point(F.ReturnX(res, 160), 160), Scalar(255, 255, 255));
	line(LaneDetect::draw, CarLocation, Point(F.ReturnX(res, LineDetect), LineDetect), Scalar(0, 0, 255));
	L.DrawLane();
	return OldAngle;
}
int CarControl::GetSpeed(const double & angle)
{
	int res = MaxSpeed - abs(angle) * 3;
	return res < MinSpeed ? MinSpeed : res;
}