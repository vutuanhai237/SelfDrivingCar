
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
#include "object_detect.h"
#include "math.h"
#include <vector>
class CarControl
{
private:
	static const double FxSize_a; // F(x) = ax + b; a=2.43095169
	static const double FxSize_b; // F(x) = ax + b; b=34.412556
	static const int LineDetect; // Location of position where the car try going to

	static const double LimitDistance; // The value represent for noise on lane detecte. The the greater value, the greater noise receive
	static const int TooFew; // The value decition when too few point to determine a lane
	static const double AngularCoefficient; // Decision how strong apply curve

	static Point CarLocation;
	static double OldAngle; // Angle that old frame return
	static const float Alpha; // The greater value, the most of change on SizeLane
	static int SizeLane; // Size of lane at line detecte
	void UpdateSizeLane(int NewSizeLane);

	static const int MinSpeed;
	static const int MaxSpeed;

  ros::NodeHandle node_obj1;
  ros::NodeHandle node_obj2;
  ros::Publisher angle_publisher;
  ros::Publisher speed_publisher;
	double Fx(int x);
public:
  CarControl();
  ~CarControl();
	double GetAngle();
	int GetSpeed(const double &angle);	
  void driverCar();

};