#include "object_detect.h"
ObjectDetect::ObjectDetect() {}
ObjectDetect::~ObjectDetect() {}
void ObjectDetect::IgnoreNoise(Line line, vector<Point> &lane)
{
	LineFunction F;
	for (int i = lane.size() - 1; i >= 0; i--)
	{
		double tmp = F.Distance(line, lane[i]);
		if (F.Distance(line, lane[i]) > LimitDistance)
			lane.erase(lane.begin() + i);
	}
}

bool ObjectDetect::FindObject(Line line, const vector<Point> &lane)
{
	if (lane.size() < 2)
		return true;
	int distance = (LaneDetect::Box.width >> 1)*LostCount;
	for (int i = 1; i < lane.size(); i++)
		if (lane[i].y - lane[i - 1].y >= LostCount)
			return true;
	return false;
}

void ObjectDetect::ClearObject()
{
	if (TrafficSign::Sign != 0)
		if (TrafficSign::Sign == 1)
			LaneDetect::LaneL.clear();
		else
			LaneDetect::LaneR.clear();
	
	LineFunction F;
	laneL = F.Linear(LaneDetect::LaneL);
	laneR = F.Linear(LaneDetect::LaneR);

	//IgnoreNoise(laneL, LaneDetect::LaneL);
	//IgnoreNoise(laneR, LaneDetect::LaneR);

	bool LostL = FindObject(laneL, LaneDetect::LaneL);
	bool LostR = FindObject(laneR, LaneDetect::LaneR);
	//if (LostL && LostR)
	//	return;
	//if (LostL)
	//	LaneDetect::LaneR.clear();
	//if (LostR)
	//	LaneDetect::LaneL.clear();
}