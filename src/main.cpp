#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/highgui/highgui.hpp>
#include "detectlane.h"
#include "carcontrol.h"
#include "TrafficSign.h"
bool STREAM = true;

Mat DetectLane::out;
bool CarControl::flag = false;

VideoCapture capture("video.avi");
//Tạo đối tượng nhận diện làn & điều khiển xe 
DetectLane *detect;
CarControl *car;
int skipFrame = 1;

void imageCallback(const sensor_msgs::ImageConstPtr &msg)
{
    //cv_bridge là namespace liên lạc giữa ROS và opencv
    //http://wiki.ros.org/cv_bridge/Tutorials/ConvertingBetweenROSImagesAndOpenCVImagesPython
    //đối tượng dưới vẫn chưa hiểu tác dụng 
    cv_bridge::CvImagePtr cv_ptr;
    //Ma trận lưu ảnh mat 
    ;//(0,0,50,50)
    try
    {
        //color image with blue-green-red color order 
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        DetectLane::out = cv_ptr->image;
        //imshow là hàm có chức năng hiển thị ảnh do con trỏ cv_ptr trỏ đến 
        cv::imshow("View", cv_ptr->image);
        //chờ gọi hàm imshow lại mỗi 1ms, giả sử waitkey(1000) thì sẽ xử lý 1 frame /s
        waitKey(1);
        //GỌi hàm update cho đối tượng detect (detect_lane)
        //
        //width, height
        cv::Rect ros; 
        ros.x = 0;
        ros.y = 0;
        ros.height = 240;
        ros.width = 320;
        //out = cv_ptr->image(ros);

        //TrafficSign sign(out);
        //cout << sign.IsTrafficSigns() << endl;

        //cout << IsTrafficSigns(out);


        detect->update(cv_ptr->image);
        //Điều khiển xe chạy với làn trái và phải, cộng thêm tốc độ cố định là 60
        //Cần điều khiển tốc độ khúc này 
        car->driverCar(detect->getLeftLane(), detect->getRightLane(), 50);
    }
    catch (cv_bridge::Exception &e)
    {
        //Bắt lỗi nếu ko nhận diện được từ camera 
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }
}

void videoProcess()
{
    Mat src;
    while (true)
    {
        //Bắt hình ảnh camera 
        capture >> src;
        if (src.empty())
            break;

        imshow("View", src);
        detect->update(src);
        waitKey(30);
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "image_listener");
    cv::namedWindow("View");
    cv::namedWindow("Binary");
    cv::namedWindow("Threshold");
    cv::namedWindow("Bird View");
    cv::namedWindow("Lane Detect");

    detect = new DetectLane();
    car = new CarControl();

    if (STREAM)
    {
        //cv::startWindowThread();
        //Tạo node xử lý 
        ros::NodeHandle nh;
        //Tạo node vận chuyển ảnh
        //image_transport của ros xử dụng để subscribe và xuất ảnh 
        //http://wiki.ros.org/image_transport
        image_transport::ImageTransport it(nh);
        //Quản lý một sub gọi  topic hình ảnh
        image_transport::Subscriber sub = it.subscribe("Team1_image", 1, imageCallback);
        //ros:spin là cho sub và publisher trong vòng lặp gửi và nhận
        ros::spin();
    }
    else
    {
        videoProcess();
    }
    cv::destroyAllWindows();
}
