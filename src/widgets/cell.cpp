#include "cell.h"

using Widgets::Cell;

Widgets::CellColor Widgets::inverseColor(Widgets::CellColor cellColor)
{
	if(cellColor == CellColor::WHITE)
		return CellColor::BLACK;
	return CellColor::WHITE;
}


Cell::Cell(CellColor color_, QPoint relativePos_): color(color_), relativePos(relativePos_)
{
	setFocusPolicy(Qt::NoFocus);
	fillWithColor();
}

void Cell::fillWithColor()
{
	const QString currentColorStr = (color == CellColor::WHITE ? Config::cellWhiteColor : Config::cellBlackColor).name();
	setStyleSheet(QString("QLabel {background-color: %1; color: red;}").arg(currentColorStr));
}
