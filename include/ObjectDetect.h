#pragma once
#include "LineFunction.h"
#include "LaneDetect.h"
#include "TrafficSign.h"

class ObjectDetect
{
#pragma region Setting value
	static const double LimitDistance; // The value represent for noise on lane detecte. The the greater value, the greater noise receive
	static const int LostCount; // noise LostCount point => have object
#pragma endregion
public:
	static Line laneL;
	static Line laneR;
private:
	void IgnoreNoise(Line line, vector<Point> &lane);
	bool FindObject(Line line, const vector<Point> &lane);
public:
	void ClearObject();
};