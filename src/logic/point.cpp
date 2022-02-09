#include "point.h"

using Logic::Point;

Point::Point(int x_, int y_)
{
	if(x < 0 || x >= Config::CELLS_IN_THE_ROW || y < 0 || y >= Config::CELLS_IN_THE_ROW)
		throw std::invalid_argument("x and y of the relative position should be in range [0, 7]");

	x = x_;
	y = y_;
}

bool Point::operator==(const Point& point) const
{
	return x == point.x && y == point.y;
}

bool Point::operator!=(const Point& point) const
{
	return !(*this == point);
}

std::string Point::toString() const
{
	return std::string(1, char('a' + x)) + std::to_string(y);
}

