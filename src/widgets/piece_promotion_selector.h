#ifndef PIECE_PROMOTION_SELECTOR_H
#define PIECE_PROMOTION_SELECTOR_H

#include <QWidget>
#include <QPoint>
#include <QHBoxLayout>
#include <QPushButton>
#include <QString>
#include <QVector>
#include <QSharedPointer>

enum class SelectedPieceType
{
	QUEEN,
	ROOK,
	BISHOP,
	KNIGHT
};

class PieceToSelectWidget: public QPushButton
{
public:
	PieceToSelectWidget(SelectedPieceType selectedPieceType_, QString whitePiecePath_, QString blackPiecePath_);

	const SelectedPieceType selectedPieceType;
	void turnToWhite();
	void turnToBlack();

	void selected();

private:
	void setBackgroundImage(QString backgroudImagePath);
	const QString whitePiecePath, blackPiecePath;
};

class PiecePromotionSelector: public QObject
{
	Q_OBJECT

signals:
	SelectedPieceType pieceSelected(SelectedPieceType selectedPieceType);

public:
	PiecePromotionSelector();
	void setParent(QWidget *parent);

	void show(QPoint newCellRelPos, bool isPiecesWhite);
	void updateGeometry(QVector<QRect> piecesGeometry);
	QPoint getCellRelPos() const;
	bool isActive() const;

private:
	QPoint cellRelPos;

	QHBoxLayout mainLayout;
	QVector<QSharedPointer<PieceToSelectWidget>> piecesToSelect;

	void showPieces();
	void hidePieces();
};

#endif // PIECE_PROMOTION_SELECTOR_H
