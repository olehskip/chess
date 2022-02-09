#ifndef BOARD_WIDGET_H
#define BOARD_WIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QVector>
#include <QSharedPointer>
#include <QResizeEvent>
#include <QtMath>
#include <limits>
#include <exception>

#include "config.h"
#include "cell.h"
#include "piece.h"
#include "piece_promotion_selector.h"

namespace Widgets
{
	class Board;
}

class Widgets::Board: public QWidget
{
	Q_OBJECT

public:
	Board();

	// when the class sends a signal that a move was made, logic class approves this move and gameWindow should call this function
	void acceptMove(QPoint pieceOriginalRelPos, QPoint pieceNewRelPos);
	// when a move was not approved by logic we should put the piece back
	void putPieceBack(QPoint pieceRelPos);
	void removePiece(QPoint pieceRelPos);
	void makePromotion(QPoint pieceRelPos);

	void startNewGame();

signals:
	// this signal indicates that the player tried to make a move, the move isn't necessarily legal
	void moveWasMade(QPoint pieceOriginalRelPos, QPoint pieceNewRelPos);
	SelectedPieceType promotionWasMade(SelectedPieceType selectedPieceType);

private:
	QVector<QVector<QSharedPointer<Widgets::Cell>>> cellWidgets;
	QVector<QSharedPointer<Widgets::Piece>> pieceWidgets;
	PiecePromotionSelector piecePromotionSelector;
	typedef decltype(pieceWidgets)::iterator PieceIt;
	QGridLayout gridLayout;

	void fillWithPieces(); // deletes all the pieces and creates new in the initial positions
	void adjustAllPiecesPosAndSize(); // calls adjustPiecePosAndSize to every piece
	void adjustPiecePosAndSize(Piece &piece); // adjust the pieces' position and size to the cells
	void resizeEvent(QResizeEvent *event);

	Widgets::Cell &getCell(QPoint cellPos); // returns a cell with cellPos, if there is no such cell then an exception is thrown
	std::optional<PieceIt> getPiece(QPoint cellPos); // returns a piece with cellPos

	// This function returns the cell's relative position, the cell is the nearest to position
	QPoint findTheNearestCell(QPoint position);
	// This funciton returns the distance between firstPoint and secondPoint
	float calculateDistance(QPoint firstPoint, QPoint secondPoint);

	/*
	 * Players can drag a piece to make a move, this function handles a signal from the Piece class and transfer a new cellPos to logic to process
	 * The function doesn't check if the move is legal
	 * pieceRelPos - is the relative position of the piece, which was dragged
	 * newAbsPos is the asbolute position where the piece was dragged to
	 */
	void manageDraggedPiece(QPoint pieceRelPos, QRect newGeometry);

	QString getPieceImagePath(SelectedPieceType selectedPieceType, bool isWhite) const;
	void updatePiecePromotionSelectorGeometry();

	void makePieceDraggable(Piece &pieceWidget);
};

#endif // BOARD_WIDGET_H
