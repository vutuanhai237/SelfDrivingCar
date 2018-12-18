#pragma once
#include "line_function.h"
#include "lane_detect.h"
#include "traffic_sign.h"

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
	ObjectDetect();
	~ObjectDetect();
	void ClearObject();
};