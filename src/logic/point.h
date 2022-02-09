#ifndef LOGIC_POINT_H
#define LOGIC_POINT_H

#include <exception>
#include <string>

#include "config.h"

namespace Logic
{
	class Point
	{
	public:
		Point() = default;
		Point(int x_, int y_);
		bool operator==(const Point& point) const;
		bool operator!=(const Point& point) const;
		std::string toString() const;

		int x = 0, y = 0;
	};
}

#endif // LOGIC_POINT_H
