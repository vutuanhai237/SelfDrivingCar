#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <iostream>

using namespace cv;
using namespace std;

static bool mirror = false;
static bool DetectRightOnly = true;
static bool ShowCam = true;

static int iIgnoreObj = 1;
static int iSizeF = 20;
static int Square = 25;

static int iLowH = 75;
static int iHighH = 130;

static int iLowS = 100;
static int iHighS = 255;

static int iLowV = 100;
static int iHighV = 255;


int IsTrafficSigns(const Mat &src);