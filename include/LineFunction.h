#include <opencv/cv.h>
using namespace cv;

struct Line // ax+by+c=0
{
	double a;
	double b;
	double c;
};

class LineFunction
{
public:
	Line CaculateLine(const Point &a, const Point &b);
	double Distance(const Line &line, const Point &p);
	double Angle(const Line &line);
	double Angle(const Point &p, const Point &CarLoction = Point(240, 240)); // This location is change
	int ReturnX(const Line &line, const double &y);
};