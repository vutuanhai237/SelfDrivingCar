#include "CarControl.h"

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

void CarControl::IgnoreNoise(Line line, vector<Point>& Lane)
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

double CarControl::Fx(int x)
{
	return FxSize_a * x + FxSize_b;
}

double CarControl::GetAngle(const vector<Point>& LaneL, const vector<Point>& LaneR, const vector<Point> &LaneM)
{
	Line res;
	LineFunction F;
	IgnoreNoise(MostAccurateLane(LaneDetect::LaneL), LaneDetect::LaneL);
	IgnoreNoise(MostAccurateLane(LaneDetect::LaneR), LaneDetect::LaneR);
	// Lost lane, kept old angle
	if (LaneL.size() + LaneR.size() < TooFew << 1)
		return OldAngle;
	// Lost left lane, follow right lane
	if (LaneL.size() < TooFew)
	{
		res = MostAccurateLane(LaneR);
		OldAngle = F.Angle(CarLocation, Point(F.ReturnX(res, LineDetect) - SizeLane >> 1, LineDetect));
		return OldAngle;
	}
	// Lost right lane, follow left lane
	if (LaneR.size() < TooFew)
	{
		res = MostAccurateLane(LaneL);
		OldAngle = F.Angle(CarLocation, Point(F.ReturnX(res, LineDetect) + SizeLane >> 1, LineDetect));
		return OldAngle;
	}
	// no lost lane
	res = MostAccurateLane(LaneM);
	//SizeLane = Fx(LineDetect)*Alpha + SizeLane * (1 - Alpha);
	//OldAngle = F.Angle(F.CaculateLine(CarLocation, Point(F.ReturnX(res, LineDetect), LineDetect)));
	Line L = MostAccurateLane(LaneL);
	Line R = MostAccurateLane(LaneR);
	UpdateSizeLane(F.ReturnX(L, LineDetect) + F.ReturnX(R, LineDetect) >> 1);

	line(LaneDetect::draw, Point(F.ReturnX(res, 0), 0), Point(F.ReturnX(res, 160), 160), Scalar(255, 0, 255));
	OldAngle = F.Angle(res) / 1.25;
	imshow("Lane detect", LaneDetect::draw);

	return OldAngle;
}


