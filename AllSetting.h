#pragma once
#include "HLS detection.h"

int HLSdetetion::OldSign = 0;
int HLSdetetion::CountFrame = 0;
bool HLSdetetion::flag = false;

int HLSdetetion::iLowH = 70;
int HLSdetetion::iHighH = 120;
int HLSdetetion::iLowL = 65;
int HLSdetetion::iHighL = 150;
int HLSdetetion::iLowS = 80;
int HLSdetetion::iHighS = 255;

#include "LaneDetect.h"
const int LaneDetect::SkyLine = 80;  // 80 = 1/3 rows
const Size LaneDetect::Box(5, 5);
const int LaneDetect::IgnoreFromMid = 80;
const float LaneDetect::alpha = 0.2;

int LaneDetect::BlurValue = 5; // 5x5
int LaneDetect::KernelSize = 3; // 3
int LaneDetect::Accuracy = 55; // 55%

vector<Point> LaneDetect::LaneL{ Point(0, 0) };
vector<Point> LaneDetect::LaneR{ Point(0, 0) };
Mat LaneDetect::draw = Mat();