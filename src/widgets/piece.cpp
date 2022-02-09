#include "piece.h"

using Widgets::Piece;

Piece::Piece(QWidget *parent, QString resourcePath, QPoint initialRelPos): QPushButton(parent)
{
	setStyleSheet(QString("QPushButton{border-image:url(%1);}").arg(resourcePath));
	setRelPos(initialRelPos);
	setMouseTracking(true);
	setFocusPolicy(Qt::NoFocus);
}

QPoint Piece::getRelPos() const
{
	return currentRelPos;
}

void Piece::setRelPos(QPoint newRelPos)
{
	if(newRelPos.x() < 0 || newRelPos.x() >= Config::CELLS_IN_THE_ROW || newRelPos.y() < 0 || newRelPos.y() >= Config::CELLS_IN_THE_ROW)
		throw std::range_error("cellPos x and y should be in range [0, 7]");
	currentRelPos = newRelPos;
}

void Piece::mousePressEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton) {
		isBeingDragged = true;
		raise();
		dragToPos(event->pos());
	}

	QWidget::mousePressEvent(event);
}
void Piece::mouseReleaseEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton) {
		isBeingDragged = false;
		emit wasDragged();
	}

	QWidget::mouseReleaseEvent(event);
}

void Piece::mouseMoveEvent(QMouseEvent *event)
{
	if(isBeingDragged) {
		dragToPos(event->pos());
	}

	QWidget::mouseMoveEvent(event);
}

void Piece::dragToPos(QPoint pos)
{
	QPoint parentPos = mapToParent(pos);
	// The cursor should point at the center of the piece
	parentPos.setX(parentPos.x() - width() / 2);
	parentPos.setY(parentPos.y() - height() / 2);
	move(parentPos);
}
