#include "car_control.h"

CarControl::CarControl()
{
	this->carPos.x = 120;
    this->carPos.y = 300;
    this->angle_publisher = node_obj1.advertise<std_msgs::Float32>("Destiny_Angle", 0);
    this->speed_publisher = node_obj2.advertise<std_msgs::Float32>("Destiny_Speed", 0);
}
CarControl::~CarControl(){}
void CarControl::driverCar(const vector<Point> &LaneL, const vector<Point> &LaneR, const vector<Point> &LaneM, int CheckSign)
{
	int i;
	float error;
    float setError;
    float velocity;
    int delta = 10;
    Point dst, predst;
    

   
	if (CheckSign == -1)
	{

	}
	else if (CheckSign == 1)
	{

	}
	else
	{
		if (LaneL[i] == LaneDetect::null && LaneR[i] == LaneDetect::null)
        {         
            i--;
            if (i < 0)
              return;
        }
        else
        {
            //Không mất làn
            if (LaneL[i] != LaneDetect::null && LaneR[i] != LaneDetect::null)
            {
                dst = (LaneL[i] + LaneR[i]) / 2;
                error = CarControl::GetAngle(LaneL, LaneR, LaneM);
            }
            //Mất làn phải
            else if (LaneL[i] != LaneDetect::null)
            {
              
            }
            //Mất làn trái
            else
            {

            }
        }
    }

    velocity = 55 - error * 3;
    if (velocity < 30)
        velocity = 30;

    std_msgs::Float32 angle;
    std_msgs::Float32 speed;

    angle.data = error;
    speed.data = velocity;

    angle_publisher.publish(angle);
    speed_publisher.publish(speed);

}
double CarControl::GetAngle(const vector<Point> &LaneL, const vector<Point> &LaneR, const vector<Point> &LaneM)
{
	Line res;
	IgnoreNoise(MostAccurateLane(LaneDetect::LaneL), LaneDetect::LaneL);
	IgnoreNoise(MostAccurateLane(LaneDetect::LaneR), LaneDetect::LaneR);
	// Lose lane, keep old angle
	if (LaneL.size() + LaneR.size() < TooFew << 1)
		return OldAngle;
	// Lost left lane, follow right lane
	if (LaneL.size() < TooFew)
	{
		res = MostAccurateLane(LaneR);
	}

	// no lost lane
	res = MostAccurateLane(LaneM);
	OldAngle = 0; // function here, code later
	return OldAngle;
}

Line CarControl::MostAccurateLane(const vector<Point> &Lane)
{
	LineFunction F; // Object for caculate
	Line line;
	Line res;
	res.a = 0;
	res.b = 0;
	res.c = 0;
	double deviation;
	double min = numeric_limits<double>::max();

	if (Lane.size() < 1)
		return res;

	// Consider each pair of points
	for (int i = 0; i < Lane.size() - 1; i++) // Unknow error: loot infinity when size of vector <= 1
	{
		for (int j = i + 1; j < Lane.size(); j++)
		{
			deviation = 0;
			// Create a linear equations
			line = F.CaculateLine(Lane[i], Lane[j]);
			// Caculate total deviation
			for (int k = 0; k < Lane.size(); k++)
				deviation += F.Distance(line, Lane[k]);
			if (deviation < min)
			{
				min = deviation;
				res = line;
			}
		}
	}
	return res;
}

void CarControl::IgnoreNoise(Line line, vector<Point> &Lane)
{
	LineFunction F;
	for (int i = Lane.size() - 1; i > 0; i--)
	{
		double tmp = F.Distance(line, Lane[i]);
		if (F.Distance(line, Lane[i]) > LimitDistance)
			Lane.erase(Lane.begin() + i);
	}
}

void CarControl::UpdateSizeLane(int NewSizeLane)
{
	SizeLane = SizeLane * (1 - Alpha) + NewSizeLane * Alpha;
}



