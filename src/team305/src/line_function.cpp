#include "line_function.h"

Line LineFunction::CaculateLine(const Point & a, const Point & b)
{
	Line line;
	if (a.x == b.x)
	{
		if (a.y == b.y)
		{
			line.a = 0;
			line.b = 0;
			line.c = 0;
			return line;
		}
		else
		{
			line.a = 1;
			line.b = 0;
			line.c = -a.x;
		}
	}
	else
	{
		if (a.y == b.y)
		{
			line.a = 0;
			line.b = 1;
			line.c = -a.y;
		}
		else
		{
			line.a = double(b.y - a.y) / (b.x - a.x);
			line.b = -1;
			line.c = double(a.y*b.x - b.y*a.x) / (b.x - a.x);
		}
	}
	return line;
}

double LineFunction::Distance(const Line &line, const Point &p)
{
	return abs(line.a * p.x + line.b * p.y + line.c) / sqrt(line.a * line.a + line.b * line.b);
}

double LineFunction::Angle(const Line & line)
{
	return atan(line.b / line.a)*(180 / acos(-1));
}

double LineFunction::Angle(const Point &p, const Point &CarLoction)
{
	double dx = p.x - CarLoction.x;
	double dy = CarLoction.y - p.y;
	return atan(dx / dy)*(180 / acos(-1));
}

int LineFunction::ReturnX(const Line &line, const double &y)
{
	return -(line.b*y + line.c) / line.a;
}

Line LineFunction::Linear(const vector<Point> &p)
{
	Line line;
	Line res;
	res.a = 0;
	res.b = 0;
	res.c = 0;
	double deviation;
	double min = numeric_limits<double>::max();

	if (p.size() < 1)
		return res;

	// Consider each pair of points
	for (int i = 0; i < p.size() - 1; i++) // Unknow error: loot infinity when size of vector <= 1
	{
		for (int j = i + 1; j < p.size(); j++)
		{
			deviation = 0;
			// Create a linear equations
			line = CaculateLine(p[i], p[j]);
			// Caculate total deviation
			for (int k = 0; k < p.size(); k++)
				deviation += Distance(line, p[k]);
			if (deviation < min)
			{
				min = deviation;
				res = line;
			}
		}
	}
	return res;
}