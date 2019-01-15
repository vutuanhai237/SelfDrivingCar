#include <setting.h>
using namespace std;
//Static variable
bool STREAM = true;
CarControl *car;
LaneDetect *detect;
TrafficSign *sign;
VideoCapture capture("video.avi");
///
void call_back(const sensor_msgs::ImageConstPtr &msg)
{
    cv_bridge::CvImagePtr cv_ptr;
    cv::Mat out;
    cv::Mat out2;
    cv::Mat out3;
    cv::Rect ros;
    try
    {
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        ///
        ros.x = 0;
        ros.y = 0;
        ros.height = 240;
        ros.width = 320;
        ///
        out = cv_ptr->image(ros);
        //imshow("camere", out);
        resize(out, out2, Size(205,154), 0, 0);
        resize(out2, out3, Size(320,240), 0, 0);
        ///
        //detect->Detect(cv_ptr->image);
        //sign->Find(cv_ptr->image);
        detect->Detect(out3);
        sign->Find(out);
        detect->DrawLane();
        car->driverCar(out3);
        ///
        imshow("View", LaneDetect::draw);
        waitKey(40);
    }
    catch (Exception e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }
}
void speed_call_back(const std_msgs::Float32::ConstPtr &msg)
{
    //ROS_INFO("I heard: [%g]", msg->data);
    car->get_speed_unity(msg->data);
}
void VideoProcess()
{
    Mat src;
    while (true)
    {
        capture >> src;
        if (src.empty())
            break;
        detect->Detect(src);
        waitKey(40);
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "image_listener");
    //detect->Setting();
    detect = new LaneDetect();
    car = new CarControl();

    if (STREAM)
    {
        ///
        ros::NodeHandle main_handle;
        ros::NodeHandle speed_handle;
        ros::Subscriber speed_unity = speed_handle.subscribe("get_speed", 10, speed_call_back);
        image_transport::ImageTransport img_trans(main_handle);
        image_transport::Subscriber sub = img_trans.subscribe("team305_image", 1, call_back);
        ///
        ros::spin();
    }
    else
    {
        VideoProcess();
    }
    cv::destroyAllWindows();

    return 0;
}