#include "CarControl.h"

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
	L.DrawLane();
	// Lost lane, kept old angle
	if (LaneDetect::LaneL.size() + LaneDetect::LaneR.size() < (LaneDetect::TooFew << 1))
		return OldAngle;
	// Lost left lane, follow right lane
	if (LaneDetect::LaneL.size() < LaneDetect::TooFew)
	{
		res = ObjectDetect::laneR;
		OldAngle = F.Angle(CarLocation, Point(F.ReturnX(res, LineDetect) - (SizeLane >> 1), LineDetect));
		line(LaneDetect::draw, CarLocation, Point(F.ReturnX(res, LineDetect) - (SizeLane >> 1), LineDetect + LaneDetect::SkyLine), Scalar(0, 0, 255));
		return OldAngle;
	}
	// Lost right lane, follow left lane
	if (LaneDetect::LaneR.size() < LaneDetect::TooFew)
	{
		res = ObjectDetect::laneL;
		OldAngle = F.Angle(CarLocation, Point(F.ReturnX(res, LineDetect) + (SizeLane >> 1), LineDetect));
		line(LaneDetect::draw, CarLocation, Point(F.ReturnX(res, LineDetect) + (SizeLane >> 1), LineDetect + LaneDetect::SkyLine), Scalar(0, 0, 255));
		return OldAngle;
	}
	// no lost lane
	L.UpdateMidLane();
	res = F.Linear(LaneDetect::LaneM);
	if (TrafficSign::Sign == 0)
		UpdateSizeLane(F.ReturnX(ObjectDetect::laneL, LineDetect) + F.ReturnX(ObjectDetect::laneR, LineDetect) >> 1);
	OldAngle = ReduceAngle(F.Angle(CarLocation, Point(F.ReturnX(res, LineDetect), LineDetect + LaneDetect::SkyLine)));
	// draw
	line(LaneDetect::draw, Point(F.ReturnX(res, 0), LaneDetect::SkyLine), Point(F.ReturnX(res, 160), 160 + LaneDetect::SkyLine), Scalar(255, 255, 255));
	line(LaneDetect::draw, CarLocation, Point(F.ReturnX(res, LineDetect), LineDetect + LaneDetect::SkyLine), Scalar(0, 0, 255));

	return OldAngle;
}

int CarControl::GetSpeed(const double & angle)
{
	int res = MaxSpeed - abs(angle) * 3;
	return res < MinSpeed ? MinSpeed : res;
}


