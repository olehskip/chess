#ifndef CELL_H
#define CELL_H

#include <QWidget>

struct Position
{
	int x, y;
};


class Cell
{
	Cell(Position cellPosition);


//private:
	const Position position;
};

#endif // CELL_H
