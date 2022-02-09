#include "board_widget.h"

using Widgets::Board;

Board::Board()
{
	gridLayout.setSpacing(0);
	setLayout(&gridLayout);

	/*
	 * We start filling the board with the top left cell, from left to right. The top cell color is white.
	 * Each row ends with the same color, which the next the row starts with
	 * Two adjacent cells have different colors in the same row
	 */
	Widgets::CellColor cellColor = Widgets::CellColor::WHITE;
	for(int x = 0; x < Config::CELLS_IN_THE_ROW; ++x) {
		cellWidgets.emplace_back();
		for(int y = 0; y < Config::CELLS_IN_THE_ROW; ++y) {
			cellWidgets[x].emplace_back(new Widgets::Cell(cellColor, QPoint(x, y)));
			gridLayout.addWidget(cellWidgets[x][y].get(), 8 - y, x);
			cellColor = inverseColor(cellColor);
		}
		cellColor = inverseColor(cellColor);
	}

	piecePromotionSelector.setParent(this);

	startNewGame();
}

void Board::acceptMove(QPoint pieceOriginalRelPos, QPoint pieceNewRelPos)
{
	const auto piece = getPiece(pieceOriginalRelPos);
	if(piece.has_value()) {
		(*piece.value())->setRelPos(pieceNewRelPos);
		adjustPiecePosAndSize(**piece.value());
	}
}

void Board::putPieceBack(QPoint pieceRelPos)
{
	const auto piece = getPiece(pieceRelPos);
	if(piece.has_value())
		adjustPiecePosAndSize(**piece.value());
}

void Board::removePiece(QPoint pieceRelPos)
{
	auto piece = getPiece(pieceRelPos);
	if(piece.has_value())
		pieceWidgets.erase(piece.value());
}

void Board::makePromotion(QPoint pieceRelPos)
{
	const auto piece = getPiece(pieceRelPos);
	if(piece.has_value()) {
		const bool isWhite = (pieceRelPos.y() == 7);
		piecePromotionSelector.show(pieceRelPos, isWhite);
		updatePiecePromotionSelectorGeometry();
		pieceWidgets.erase(piece.value());
		connect(&piecePromotionSelector, &PiecePromotionSelector::pieceSelected, this, [isWhite, pieceRelPos, this](SelectedPieceType selectedPieceType)
		{
			pieceWidgets.emplace_back(new Piece(this, getPieceImagePath(selectedPieceType, isWhite), pieceRelPos));
			makePieceDraggable(*pieceWidgets.back());
			pieceWidgets.back()->show();
			adjustPiecePosAndSize(*pieceWidgets.back());
			emit promotionWasMade(selectedPieceType);
			disconnect(&piecePromotionSelector, &PiecePromotionSelector::pieceSelected, 0, 0);
		});
	}
}

void Board::startNewGame()
{
	fillWithPieces();
}

void Board::fillWithPieces()
{
	pieceWidgets.clear();
	pieceWidgets.emplace_back(new Piece(this, Config::PIECE_PATHS::WHITE_ROOK, QPoint(0, 0)));
	pieceWidgets.emplace_back(new Piece(this, Config::PIECE_PATHS::WHITE_KNIGHT, QPoint(1, 0)));
	pieceWidgets.emplace_back(new Piece(this, Config::PIECE_PATHS::WHITE_BISHOP, QPoint(2, 0)));
	pieceWidgets.emplace_back(new Piece(this, Config::PIECE_PATHS::WHITE_QUEEN, QPoint(3, 0)));
	pieceWidgets.emplace_back(new Piece(this, Config::PIECE_PATHS::WHITE_KING, QPoint(4, 0)));
	pieceWidgets.emplace_back(new Piece(this, Config::PIECE_PATHS::WHITE_BISHOP, QPoint(5, 0)));
	pieceWidgets.emplace_back(new Piece(this, Config::PIECE_PATHS::WHITE_KNIGHT, QPoint(6, 0)));
	pieceWidgets.emplace_back(new Piece(this, Config::PIECE_PATHS::WHITE_ROOK, QPoint(7, 0)));
	for(int x = 0; x < Config::CELLS_IN_THE_ROW; ++x) {
		pieceWidgets.emplace_back(new Piece(this, Config::PIECE_PATHS::WHITE_PAWN, QPoint(x, 1)));
	}

	pieceWidgets.emplace_back(new Piece(this, Config::PIECE_PATHS::BLACK_ROOK, QPoint(0, 7)));
	pieceWidgets.emplace_back(new Piece(this, Config::PIECE_PATHS::BLACK_KNIGHT, QPoint(1, 7)));
	pieceWidgets.emplace_back(new Piece(this, Config::PIECE_PATHS::BLACK_BISHOP, QPoint(2, 7)));
	pieceWidgets.emplace_back(new Piece(this, Config::PIECE_PATHS::BLACK_QUEEN, QPoint(3, 7)));
	pieceWidgets.emplace_back(new Piece(this, Config::PIECE_PATHS::BLACK_KING, QPoint(4, 7)));
	pieceWidgets.emplace_back(new Piece(this, Config::PIECE_PATHS::BLACK_BISHOP, QPoint(5, 7)));
	pieceWidgets.emplace_back(new Piece(this, Config::PIECE_PATHS::BLACK_KNIGHT, QPoint(6, 7)));
	pieceWidgets.emplace_back(new Piece(this, Config::PIECE_PATHS::BLACK_ROOK, QPoint(7, 7)));
	for(int x = 0; x < Config::CELLS_IN_THE_ROW; ++x) {
		pieceWidgets.emplace_back(new Piece(this, Config::PIECE_PATHS::BLACK_PAWN, QPoint(x, 6)));
	}

	for(auto &pieceWidget: pieceWidgets) {
		makePieceDraggable(*pieceWidget);
		pieceWidget->show();
	}

	adjustAllPiecesPosAndSize();
}

void Board::adjustAllPiecesPosAndSize()
{
	for(auto &piece: pieceWidgets) {
		adjustPiecePosAndSize(*piece);
	}
}

void Board::adjustPiecePosAndSize(Piece &piece)
{
	const QPoint pieceRelPos = piece.getRelPos();
	piece.resize(cellWidgets[pieceRelPos.x()][pieceRelPos.y()]->size());
	piece.move(cellWidgets[pieceRelPos.x()][pieceRelPos.y()]->pos());
	updatePiecePromotionSelectorGeometry();
}

void Board::resizeEvent(QResizeEvent *event)
{
	adjustAllPiecesPosAndSize();
	QWidget::resizeEvent(event);
}

Widgets::Cell &Board::getCell(QPoint cellPos)
{
	return *cellWidgets.at(cellPos.x()).at(cellPos.y());
}

std::optional<Board::PieceIt> Board::getPiece(QPoint cellPos)
{
	for(auto it = pieceWidgets.begin(); it != pieceWidgets.end(); ++it) {
		if((*it)->getRelPos() == cellPos)
			return it;

	}
	return std::nullopt;
}

QPoint Board::findTheNearestCell(QPoint position)
{
	QPoint nearestCellRelPos;
	float nearestDistance = std::numeric_limits<float>::max();
	for(auto &vec: cellWidgets) {
		for(auto &cell: vec) {
			const float currentCellDistance = calculateDistance(cell->pos(), position);
			if(currentCellDistance < nearestDistance) {
				nearestCellRelPos = cell->relativePos;
				nearestDistance = currentCellDistance;
			}
		}
	}
	return nearestCellRelPos;
}

float Board::calculateDistance(QPoint firstPoint, QPoint secondPoint)
{
	return qSqrt(qPow(secondPoint.x() - firstPoint.x(), 2) + qPow(secondPoint.y() - firstPoint.y(), 2));
}

void Board::manageDraggedPiece(QPoint pieceRelPos, QRect newGeometry)
{
	// if the player drags a piece out of the board, we should cancel that move and return the piece to its original position
	if(!geometry().contains(newGeometry.center())) {
		adjustPiecePosAndSize(**getPiece(pieceRelPos).value());
		return;
	}

	QPoint theNearestCellRelPos = findTheNearestCell(newGeometry.topLeft()); // the nearest cell to newAbsPos
	// if the new and old position are the same there is no need to tranfer it to logic, we can just return the piece to its original position
	if(theNearestCellRelPos == pieceRelPos) {
		adjustPiecePosAndSize(**getPiece(theNearestCellRelPos).value());
		return;
	}

	// tranfer the move to logic through gameWindow to be processed
	emit moveWasMade(pieceRelPos, theNearestCellRelPos);
}

QString Board::getPieceImagePath(SelectedPieceType selectedPieceType, bool isWhite) const
{
	if(selectedPieceType == SelectedPieceType::QUEEN)
		return (isWhite ? Config::PIECE_PATHS::WHITE_QUEEN : Config::PIECE_PATHS::BLACK_QUEEN);
	else if(selectedPieceType == SelectedPieceType::ROOK)
		return (isWhite ? Config::PIECE_PATHS::WHITE_ROOK : Config::PIECE_PATHS::BLACK_ROOK);
	else if(selectedPieceType == SelectedPieceType::BISHOP)
		return (isWhite ? Config::PIECE_PATHS::WHITE_BISHOP : Config::PIECE_PATHS::BLACK_BISHOP);
	else if(selectedPieceType == SelectedPieceType::KNIGHT)
		return (isWhite ? Config::PIECE_PATHS::WHITE_KNIGHT : Config::PIECE_PATHS::BLACK_KNIGHT);

	throw std::logic_error("getPieceImagePath exception");
}

void Board::updatePiecePromotionSelectorGeometry()
{
	if(!piecePromotionSelector.isActive())
		return;

	const auto cellPos = piecePromotionSelector.getCellRelPos();
	const int deltaY = (cellPos.y() == 0 ? 1 : -1);
	QVector<QRect> piecesGeometry;
	for(int y = cellPos.y(); piecesGeometry.size() < 4; y += deltaY) {
		piecesGeometry.push_back(getCell(QPoint(cellPos.x(), y)).geometry());
	}
	piecePromotionSelector.updateGeometry(piecesGeometry);
}

void Board::makePieceDraggable(Piece &pieceWidget)
{
	connect(&pieceWidget, &Piece::wasDragged, this, [this]()
	{
		const Piece *piece = qobject_cast<Widgets::Piece*>(sender());
		manageDraggedPiece(piece->getRelPos(), piece->geometry());
	});
}
