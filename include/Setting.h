#pragma once
/*   ---TrafficSign.h---   */
#include "TrafficSign.h"
int TrafficSign::OldSign = 0;
int TrafficSign::CountFrame = 0;
bool TrafficSign::flag = false;
// threshold of sign
int TrafficSign::iLowH = 70;
int TrafficSign::iHighH = 120;
int TrafficSign::iLowL = 65;
int TrafficSign::iHighL = 150;
int TrafficSign::iLowS = 80;
int TrafficSign::iHighS = 255;

/*   ---LaneDetect.h---   */
#include "LaneDetect.h"
const int LaneDetect::SkyLine = 80;  // 80 = 1/3 rows
const Size LaneDetect::Box(5, 5);
const float LaneDetect::alpha = 0.2;

int LaneDetect::BlurValue = 5; // 5x5
int LaneDetect::KernelSize = 3; // 3
int LaneDetect::Accuracy = 55; // 55%

double LaneDetect::Fx_a = double(6) / 13; //F(x) = ax + b; a = 5/13
double LaneDetect::Fx_b = double(370) / 13; //F(x) = ax + b; b = 395/13

vector<Point> LaneDetect::LaneL{ Point(0, 0) };
vector<Point> LaneDetect::LaneR{ Point(0, 0) };
vector<Point> LaneDetect::LaneM{ Point(0, 0) };
Mat LaneDetect::draw = Mat();

/*   ---CarControl.h---   */
#include "CarControl.h"
const double CarControl::FxSize_a = 2.43095169; // F(x) = ax + b; a=2.43095169
const double CarControl::FxSize_b = 34.412556; // F(x) = ax + b; b=34.412556
const int CarControl::LineDetect = 50; // Location of Y-axis position where the car try going to

const double CarControl::LimitDistance = 3.0; // The value represent for noise on lane detecte. The the greater value, the greater noise receive
const int CarControl::TooFew = 3; // The value decition when too few point to determine a lane
const double CarControl::AngularCoefficient = 0.8; // Decision how strong apply curve

Point CarControl::CarLocation = Point(320 >> 1, 160);

double CarControl::OldAngle = 0; // Angle that old frame return
const float CarControl::Alpha = 0.25; // the greater value, the most of change on SizeLane
int CarControl::SizeLane = 40; // Size of lane