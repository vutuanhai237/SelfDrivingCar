#include <setting.h>

using namespace std;

//Static variable 
bool STREAM = true;
CarControl *car;
LaneDetect *detect;
TrafficSign *sign;
VideoCapture capture("video.avi");



void call_back(const sensor_msgs::ImageConstPtr &msg)
{
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
        cv::Mat out;
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
         cv::Rect ros; 
        ros.x = 0;
        ros.y = 0;
        ros.height = 240;
        ros.width = 320;

        out = cv_ptr->image(ros);
        //cv::imshow("View", cv_ptr->image);
        waitKey(33);
        detect->Detect(cv_ptr->image);
        sign->Find(cv_ptr->image);

        detect->DrawLane();
       
        car->driverCar(out);
        imshow("View", LaneDetect::draw);
    }
    catch(Exception e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }
}

void VideoProcess()
{
    Mat src;
    while (true)
    {
        capture >> src;
        if (src.empty())
            break;
        //imshow("View", src);

        detect->Detect(src);
        waitKey(33);
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "image_listener");
    //cv::namedWindow("Binary");
    //cv::namedWindow("Lane Detect");
    //TrafficSign *F;
    //F->Setting();
    detect->Setting();
    detect = new LaneDetect();
    car = new CarControl();
    
    if (STREAM)
    {  
        ros::NodeHandle main_handle;   
        image_transport::ImageTransport img_trans(main_handle);
        image_transport::Subscriber sub = img_trans.subscribe("Team1_image", 1, call_back);
        ros::spin();
    }
    else
    {
        VideoProcess();
    }
    cv::destroyAllWindows();

    return 0;
}