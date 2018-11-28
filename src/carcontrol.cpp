#include "carcontrol.h"

CarControl::CarControl()
{
    //Tham số của bánh lái, cần xem kĩ chỗ này
    //carPos.x nều > 120 thì xe sẽ quẹo phải và ngược lại
    //carPos.x nếu < 300 thì xe sẽ quẹo phải và ngược lại
    carPos.x = 120;
    carPos.y = 300;
    steer_publisher = node_obj1.advertise<std_msgs::Float32>("Team1_steerAngle", 10);
    speed_publisher = node_obj2.advertise<std_msgs::Float32>("Team1_speed", 10);
}

CarControl::~CarControl() {}
//getAngleLane
double getanglelane()
{
    return 0;
}
float CarControl::errorAngle(const Point &dst)
{
    //dst là center point cần phải đi đến
    //nếu xe đã ở vị trí center point x thì ko cần phải xoay bánh lái nữa
    if (dst.x == carPos.x)
        return 0;
    if (dst.y == carPos.y)
        return (dst.x < carPos.x ? -90 : 90);
    // lấy pi
    double pi = acos(-1.0);
    double dx = dst.x - carPos.x;
    double dy = carPos.y - dst.y;
    //tính góc xoay
    if (dx < 0)
        return -atan(-dx / dy) * 180 / pi;
    return atan(dx / dy) * 180 / pi;
}
int i;
int counter_left = 0;
int counter_right = 0;
void CarControl::driverCar(const vector<Point> &left, const vector<Point> &right, float velocity)
{
    Point preLeft, preRight;
    Point dst, predst;
    if (left.size() <= right.size())
    {
        i = left.size() - 5;
    }
    else
    {
        i = right.size() - 5;
    }

    float error;
    float setError;
    float preVelocity;
    int delta = 10;

    int sign = 0; //Sign recoz
    if (sign == 0)
    {
        if (counter_left != 0)
            counter_left--;
        if (counter_right != 0)
            counter_right--;
    }
    //if (sign == 1)
    //{
    //    counter_right = 6;
    //   counter_left = 0;
    //}
    //if (sign == -1)
    //{
    //    counter_left = 6;
    //    counter_right = 0;
    //}
    //No left no right
    if (counter_left == 0 && counter_right == 0)
    {

        //Mất 2 làn
        if (left[i] == DetectLane::null && right[i] == DetectLane::null)
        {
            //laneWidth = (laneWidth + preRight.x - preLeft.x) / 2;
            //dst = (preLeft + preRight) / 2;
            //error = errorAngle(dst);
            /*if (flag == false)
            {

                flag = true;
                DetectLane *detect2;
                detect2->Fix_Thres();
                //cv_bridge::CvImagePtr cv_ptr;
                //  cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
                //detect2->update(detect2->out);
                //CarControl::driverCar(detect2->getLeftLane(), detect2->getRightLane(), 50);
                detect2->Def_Thres();
                flag = false;
            }*/
            //error = preError;
            //error = preError + 15;
            i--;
            if (i < 0)
              return;
        }
        else
        {
            //Không mất làn
            if (left[i] != DetectLane::null && right[i] != DetectLane::null)
            {
                dst = (left[i] + right[i]) / 2;

                error = errorAngle(dst);
                setError = error;
                laneWidth = (laneWidth + right[i].x - left[i].x) / 2;
            }
            //Mất làn phải
            else if (left[i] != DetectLane::null)
            {
                //laneWidth = (laneWidth + preRight.x - left[i].x) / 2;
                dst.x = left[i].x + int(laneWidth) * 1 / 2 - 20;
                //dst.x = predst.x + preRight.x - right[i].x;
                dst.y = 5;
                error = errorAngle(dst);
                setError = error;
            }
            //Mất làn trái
            else
            {

                dst.x = right[i].x - int(laneWidth) * 1 / 2 + 20;
                //dst.x = predst.x + right[i].x - preRight.x;
                dst.y = 5;
                error = errorAngle(dst);
                setError = error;
            }
        }
    }
    //Biển báo rẽ phải bám làn phải
    else if (counter_right != 0 && counter_left == 0)
    {
        counter_right--;
        error = 60;
    }
    //Biển báo rẽ trái bám làn trái
    else
    {
        counter_right--;
        error = 60;
    }

    if (left[i] != DetectLane::null)
    {
        preLeft = left[i];
    }
    if (right[i] != DetectLane::null)
    {
        preRight = right[i];
    }
    preError = error;
    preVelocity = velocity;
    predst = dst;
    velocity = 55 - error * 3;
    if (velocity < 30)
        velocity = 30;

    std_msgs::Float32 angle;
    std_msgs::Float32 speed;

    angle.data = error;
    speed.data = velocity;

    steer_publisher.publish(angle);
    speed_publisher.publish(speed);
}
