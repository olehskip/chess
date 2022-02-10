#ifndef CELL_H
#define CELL_H

#include <QPoint>
#include <QLabel>
#include <QColor>
#include <QString>

#include "config.h"

namespace Widgets
{
	class Cell;
	enum class CellColor: bool
	{
		WHITE,
		BLACK
	};

	CellColor inverseColor(CellColor cellColor);

};

class Widgets::Cell: public QLabel
{
public:
	Cell(CellColor color_, QPoint relativePos_);

	const CellColor color;
	const QPoint relativePos;

private:
	void fillWithColor(); // change the QLabel color to the cell color
};

#endif // CELL_H
