#include "line_function.h"


Line LineFunction::CaculateLine(const Point &a, const Point &b)
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
			line.a = b.y - a.y;
			line.b = a.x - b.x;
			line.c = -(a.y * (a.x - b.x) + a.x * (b.y - a.y));
		}
	}
	return line;
}

double LineFunction::Distance(const Line &line, const Point &p)
{
	return abs(line.a * p.x + line.b * p.y + line.c) / sqrt(line.a * line.a + line.b * line.b);
}

double LineFunction::Angle(const Line &line)
{
	return -atan(-line.a / line.b) * (180 / acos(-1.0));
}

double LineFunction::Angle(const Point &p, const Point &CarLoction)
{
	double dx = CarLoction.x - p.x;
	double dy = CarLoction.y - p.y;
	return -atan(dx / dy) * (180 / acos(-1));
}
