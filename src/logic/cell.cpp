#include "cell.h"

using Logic::Cell;

Cell::Cell(Point relPos_): relPos(relPos_)
{

}

bool Cell::getIsOccupied() const
{
	return isOccupied;
}

void Cell::occupy()
{
	isOccupied = true;
}

void Cell::setFree()
{
	isOccupied = false;
}
