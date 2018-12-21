#pragma once
#include "LineFunction.h"
#include "LaneDetect.h"
#include "ObjectDetect.h"
#include "LaneDetect.h"

class CarControl
{
private:
	static const double FxSize_a; // F(x) = ax + b; a=2.43095169
	static const double FxSize_b; // F(x) = ax + b; b=34.412556
	static const int LineDetect; // Location of position where the car try going to

	static const double LimitDistance; // The value represent for noise on lane detecte. The the greater value, the greater noise receive

	static const double AngularCoefficient; // Decision how strong apply curve

	static Point CarLocation;
	static double OldAngle; // Angle that old frame return
	static const float Alpha; // The greater value, the most of change on SizeLane
	static int SizeLane; // Size of lane at line detecte
	void UpdateSizeLane(int NewSizeLane);
public:
	static const double TurnStrong; // Default is 1.5; > 1.5 mean turn more effect; < 1.5 mean turn least effect
private:
	static const int MinSpeed;
	static const int MaxSpeed;
	double Fx(int x);
	double ReduceAngle(const double &angle);
public:
	double GetAngle();
	int GetSpeed(const double &angle);	
};