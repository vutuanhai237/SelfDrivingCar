#include "CarControl.h"

void CarControl::UpdateSizeLane(int NewSizeLane)
{
	SizeLane = SizeLane * (1 - Alpha) + NewSizeLane * Alpha;
}

double CarControl::Fx(int x)
{
	return FxSize_a * x + FxSize_b;
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
		OldAngle = F.Angle(CarLocation, Point(F.ReturnX(res, LineDetect) - (SizeLane >> 2), LineDetect));
		line(LaneDetect::draw, CarLocation, Point(F.ReturnX(res, LineDetect) - (SizeLane >> 2), LineDetect), Scalar(0, 0, 255));
		return OldAngle;
	}
	// Lost right lane, follow left lane
	if (LaneDetect::LaneR.empty())
	{
		res = ObjectDetect::laneL;
		OldAngle = F.Angle(CarLocation, Point(F.ReturnX(res, LineDetect) + (SizeLane >> 2), LineDetect));
		line(LaneDetect::draw, CarLocation, Point(F.ReturnX(res, LineDetect) + (SizeLane >> 2), LineDetect), Scalar(0, 0, 255));
		return OldAngle;
	}
	// no lost lane
	L.UpdateMidLane();
	res = F.Linear(LaneDetect::LaneM);
	UpdateSizeLane(F.ReturnX(ObjectDetect::laneL, LineDetect) + F.ReturnX(ObjectDetect::laneR, LineDetect) >> 1);
	OldAngle = F.Angle(CarLocation, Point(F.ReturnX(res, LineDetect), LineDetect));
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


