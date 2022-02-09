#include "piece_promotion_selector.h"

// ---PieceToSelectWidget---
PieceToSelectWidget::PieceToSelectWidget(SelectedPieceType selectedPieceType_, QString whitePiecePath_, QString blackPiecePath_):
	selectedPieceType(selectedPieceType_), whitePiecePath(whitePiecePath_), blackPiecePath(blackPiecePath_)
{
	setContentsMargins(0, 0, 0, 0);
}

void PieceToSelectWidget::turnToWhite()
{
	setBackgroundImage(whitePiecePath);
}

void PieceToSelectWidget::turnToBlack()
{
	setBackgroundImage(blackPiecePath);
}

void PieceToSelectWidget::setBackgroundImage(QString backgroudImagePath)
{
	setStyleSheet(QString("QPushButton{background-color: gray; border-image:url(%1);} QPushButton::hover{background-color:#9c9797;}").arg(backgroudImagePath));
}

// ---PiecePromotionSelector---
PiecePromotionSelector::PiecePromotionSelector()
{
//	setLayout(&mainLayout);
	piecesToSelect.emplace_back(new PieceToSelectWidget(SelectedPieceType::QUEEN, ":/white_pieces/queen.png", ":/black_pieces/queen.png"));
	piecesToSelect.emplace_back(new PieceToSelectWidget(SelectedPieceType::ROOK, ":/white_pieces/rook.png", ":/black_pieces/rook.png"));
	piecesToSelect.emplace_back(new PieceToSelectWidget(SelectedPieceType::BISHOP, ":/white_pieces/bishop.png", ":/black_pieces/bishop.png"));
	piecesToSelect.emplace_back(new PieceToSelectWidget(SelectedPieceType::KNIGHT, ":/white_pieces/knight.png", ":/black_pieces/knight.png"));

	for(auto &piece: piecesToSelect) {
		piece->setVisible(false);
		connect(piece.get(), &PieceToSelectWidget::clicked, this, [&, selectedPieceType=piece->selectedPieceType]()
		{
			emit pieceSelected(selectedPieceType);
//			hide();
			hidePieces();
		});
	}
	mainLayout.setSpacing(0);
	mainLayout.setContentsMargins(0, 0, 0, 0);
//	hide();
//	setStyleSheet("QWidget{background-color: gray;}");
}

void PiecePromotionSelector::setParent(QWidget *parent)
{
	for(auto &piece: piecesToSelect) {
		piece->setParent(parent);
	}
}

void PiecePromotionSelector::show(QPoint newCellRelPos, bool isPiecesWhite)
{
	cellRelPos = newCellRelPos;

	for(int i = 0; i < piecesToSelect.size(); ++i) {
		if(isPiecesWhite)
			piecesToSelect[i]->turnToWhite();
		else
			piecesToSelect[i]->turnToBlack();
	}
	showPieces();
}

void PiecePromotionSelector::updateGeometry(QVector<QRect> piecesGeometry)
{
	if(piecesGeometry.size() != piecesToSelect.size())
		throw std::length_error("piecesSize.size() != piecesToSelect.size()");

	for(int i = 0; i < piecesGeometry.size(); ++i) {
		piecesToSelect[i]->setGeometry(piecesGeometry[i]);
	}
}

QPoint PiecePromotionSelector::getCellRelPos() const
{
	return cellRelPos;
}

bool PiecePromotionSelector::isActive() const
{
	return piecesToSelect.empty() || piecesToSelect[0]->isVisible();
}

void PiecePromotionSelector::showPieces()
{
	for(auto &piece: piecesToSelect) {
		piece->show();
		piece->raise();
	}
}

void PiecePromotionSelector::hidePieces()
{
	for(auto &piece: piecesToSelect) {
		piece->hide();
	}
}
