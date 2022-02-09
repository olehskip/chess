#ifndef WIDGETS_PIECE_H
#define WIDGETS_PIECE_H

#include <QPushButton>
#include <QString>
#include <QPoint>
#include <QMouseEvent>
#include <QEnterEvent>
#include <exception>

#include "config.h"

namespace Widgets
{
	class Piece;
};

class Widgets::Piece: public QPushButton
{
	Q_OBJECT

public:
	Piece(QWidget *parent, QString resourcePath, QPoint initialRelPos);

	QPoint getRelPos() const;
	void setRelPos(QPoint newRelPos);

signals:
	void wasDragged();

private:
	QPoint currentRelPos;

	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void dragToPos(QPoint pos);
	bool isBeingDragged = false;
};

#endif // WIDGETS_PIECE_H
