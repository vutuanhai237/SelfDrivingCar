#pragma once
//include 
// -- ROS Header 
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <std_msgs/Float32.h>
// -- Other Header 
#include "line_function.h"
#include "lane_detect.h"
#include "math.h"
#include <vector>

using namespace std;
class CarControl
{
  private:
    //Static variable 
    static const double LimitDistance;      // The value represent for noise on lane detecte. The the greater value, the greater noise receive
    static const int TooFew;                // The value decition when too few point to determine a lane
    static const double AngularCoefficient; // Decision how strong apply curve
    static double OldAngle;                 // Angle that old frame return
    static const float Alpha;               // The greater value, the most of change on SizeLane
    static const int LineDetece;            // Location of line cut across Mat input that decision when car want go
    static int SizeLane;                    // Size of lane at line detecte
    //Private function 
    Line MostAccurateLane(const vector<Point> &Lane);
    void IgnoreNoise(Line line, vector<Point> &Lane);
    void UpdateSizeLane(int NewSizeLane);
    Point carPos;
    //ROS Publisher & Subscriber 
    ros::NodeHandle node_obj1;
    ros::NodeHandle node_obj2;
    ros::Publisher angle_publisher;
    ros::Publisher speed_publisher;

  public:
    CarControl();
    ~CarControl();
    double GetAngle(const vector<Point> &LaneL, const vector<Point> &LaneR, const vector<Point> &LaneM);
    void driverCar(const vector<Point> &LaneL, const vector<Point> &LaneR, const vector<Point> &LaneM, int CheckSign);
};