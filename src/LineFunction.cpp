#include "LineFunction.h"

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
	return -atan(dx / dy)*(180 / acos(-1));
}

int LineFunction::ReturnX(const Line &line, const double &y)
{
	return -(line.b*y + line.c) / line.a;
}
