#ifndef LOGIC_CELL_H
#define LOGIC_CELL_H

#include "point.h"

namespace Logic
{
	class Cell;
};

class Logic::Cell
{
public:
	Cell(Point relPos_);
	bool getIsOccupied() const;
	void occupy();
	void setFree();

	const Point relPos;
private:
	bool isOccupied = false;
};

#endif // LOGIC_CELL_H
