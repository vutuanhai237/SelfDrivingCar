#ifndef DETECTLANE_H
#define DETECTLANE_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <ros/ros.h>
#include <vector>
#include <math.h>
#include <algorithm>

using namespace std;
using namespace cv;

class DetectLane
{
public:
    DetectLane();
    ~DetectLane();

    void update(const Mat &src);
    
    vector<Point> getLeftLane();
    vector<Point> getRightLane();

    static int slideThickness;

    static int BIRDVIEW_WIDTH;
    static int BIRDVIEW_HEIGHT;

    static int VERTICAL;
    static int HORIZONTAL;

    static Point null; // 


    static Mat out;
    void Def_Thres();
    void Fix_Thres();
private:
    //Tạo bird view & binary 
    Mat preProcess(const Mat &src);
    //???
    Mat morphological(const Mat &imgHSV);
    //transform sang lane_detect?
    Mat birdViewTranform(const Mat &source);
    void fillLane(Mat &src);
    //Chia lớp?
    vector<Mat> splitLayer(const Mat &src, int dir = VERTICAL);
    //Bám trung tâm đường 
    vector<vector<Point> > centerRoadSide(const vector<Mat> &src, int dir = VERTICAL);
    //Nhận diện làn đường trái phải 
    void detectLeftRight(const vector<vector<Point> > &points);
    //Xử lý làn trong trường hợp bóng 
    Mat laneInShadow(const Mat &src);
    //Các tham số phải tùy chỉnh cho đề bài 
    int minThreshold[3] = {50, 0, 150};

    int maxThreshold[3] = {179, 255, 255};
    int minShadowTh[3] = {90, 43, 36};
    int maxShadowTh[3] = {120, 81, 171};
    int minLaneInShadow[3] = {90, 43, 97};
    int maxLaneInShadow[3] = {120, 80, 171};
    int binaryThreshold = 255;

    int skyLine = 85;
    int shadowParam = 40;
    //dùng để tạo các điểm trên lane_detect 
    vector<Point> leftLane, rightLane;
};

#endif
