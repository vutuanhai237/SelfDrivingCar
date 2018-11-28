#ifndef CARCONTROL_H
#define CARCONTROL_H
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <ros/ros.h>
#include "std_msgs/Float32.h"

#include <vector>
#include <math.h>

#include "detectlane.h"

using namespace std;
using namespace cv;

class CarControl 
{
public:
    CarControl();
    ~CarControl();
    static bool flag; 

    void driverCar(const vector<Point> &left, const vector<Point> &right, float velocity);

private:
    //Hàm điều chỉnh góc ái 
    float errorAngle(const Point &dst);
    //Các node dùng để gửi msg
    ros::NodeHandle node_obj1;
    ros::NodeHandle node_obj2;
    ros::Publisher steer_publisher;
    ros::Publisher speed_publisher;
    //Lưu vị trí xe
    Point carPos;
    //Độ rộng làn đường 
    float laneWidth = 60;
    //Tốc độ 
    float minVelocity = 30;
    float maxVelocity = 50;

    float preError;

    //Các tham số để thực hiện một chu trình điều khiển PID 
    float kP;
    float kI;
    float kD;
    int t_kP;
    int t_kI;
    int t_kD;
};

#endif
