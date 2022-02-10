#include "game_logic.h"

using Logic::GameLogic;

GameLogic::GameLogic()
{
	startNewGame();
}

void GameLogic::startNewGame()
{
	currentTurn = PlayerColor::WHITE;
	gameStatus = GameStatus::ACTIVE;
	fillCells();
	fillPieces();
}

Logic::MoveResponse GameLogic::makeMove(Point oldRelPos, Point newRelPos)
{
	MoveResponse moveResponse;
	if(gameStatus != GameStatus::ACTIVE) {
		return moveResponse;
	}

	auto piece = getPiece(oldRelPos).value();

	Cell &newCell = getCell(newRelPos), &oldCell = getCell(oldRelPos);
	moveResponse.isMoveApproved = true;
	if(isMovePossible(**piece, newRelPos)) {
		moveResponse.notationText = (*piece)->notationSymbol + oldRelPos.toString() + newRelPos.toString();
		moveResponse.posChanges = {{oldRelPos, newRelPos}};

		(*piece)->setRelPos(newRelPos);
		newCell.occupy();

	}
	else if(isCapturePossible(**piece, newRelPos)) {
		auto capturedPiece = getPiece(newRelPos).value();
		moveResponse.isPieceWasCaptured = true;
		moveResponse.notationText = (*piece)->notationSymbol + oldRelPos.toString() + "X" + (*capturedPiece)->notationSymbol + newRelPos.toString();
		moveResponse.posChanges = {{oldRelPos, newRelPos}};

		pieces.erase(capturedPiece);
		(*piece)->setRelPos(newRelPos);
		newCell.occupy();
	}
	else if(auto newPositions = isCastlePossible(**piece, newRelPos); newPositions.has_value()) {
		auto rook = getPiece(newRelPos).value();
		moveResponse.notationText = std::string("0-0") + std::string((*rook)->getRelPos().x == 0 ? "-0" : "");
		moveResponse.posChanges = {{oldRelPos, newPositions->first}, {(*rook)->getRelPos(), newPositions->second}};

		newCell.setFree();
		(*piece)->setRelPos(newPositions->first);
		(*rook)->setRelPos(newPositions->second);
		getCell(newPositions->first).occupy();
		getCell(newPositions->second).occupy();
	}
	else
		moveResponse.isMoveApproved = false;

	if(moveResponse.isMoveApproved) {
		switchTurn();
		oldCell.setFree();

		checkForCheckmate() || checkForStalemate();
		if(gameStatus == GameStatus::ACTIVE && (*piece)->canPromote(newRelPos)) {
			gameStatus = GameStatus::PROMOTION_REQUIRED;
			promotionRelPos = newRelPos;
		}
	}

	return moveResponse;
}

Logic::GameStatus GameLogic::getGameStatus() const
{
	return gameStatus;
}

void GameLogic::fillPieces()
{
	pieces.clear();
	pieces.emplace_back(new Rook(PlayerColor::WHITE, Point(0, 0)));
	pieces.emplace_back(new Knight(PlayerColor::WHITE, Point(1, 0)));
	pieces.emplace_back(new Bishop(PlayerColor::WHITE, Point(2, 0)));
	pieces.emplace_back(new Queen(PlayerColor::WHITE, Point(3, 0)));
	pieces.emplace_back(new King(PlayerColor::WHITE, Point(4, 0)));
	pieces.emplace_back(new Bishop(PlayerColor::WHITE, Point(5, 0)));
	pieces.emplace_back(new Knight(PlayerColor::WHITE, Point(6, 0)));
	pieces.emplace_back(new Rook(PlayerColor::WHITE, Point(7, 0)));
	for(int x = 0; x < Config::CELLS_IN_THE_ROW; ++x) {
		pieces.emplace_back(new Pawn(PlayerColor::WHITE, Point(x, 1)));
	}

	pieces.emplace_back(new Rook(PlayerColor::BLACK, Point(0, 7)));
	pieces.emplace_back(new Knight(PlayerColor::BLACK, Point(1, 7)));
	pieces.emplace_back(new Bishop(PlayerColor::BLACK, Point(2, 7)));
	pieces.emplace_back(new Queen(PlayerColor::BLACK, Point(3, 7)));
	pieces.emplace_back(new King(PlayerColor::BLACK, Point(4, 7)));
	pieces.emplace_back(new Bishop(PlayerColor::BLACK, Point(5, 7)));
	pieces.emplace_back(new Knight(PlayerColor::BLACK, Point(6, 7)));
	pieces.emplace_back(new Rook(PlayerColor::BLACK, Point(7, 7)));
	for(int x = 0; x < Config::CELLS_IN_THE_ROW; ++x) {
		pieces.emplace_back(new Pawn(PlayerColor::BLACK, Point(x, 6)));
	}

	for(auto &piece: pieces) {
		getCell(piece->getRelPos()).occupy();
	}
}

void GameLogic::fillCells()
{
	cells.clear();
	for(int x = 0; x < Config::CELLS_IN_THE_ROW; ++x) {
		cells.emplace_back();
		for(int y = 0; y < Config::CELLS_IN_THE_ROW; ++y) {
			cells[x].emplace_back(new Cell(Point(x, y)));
		}
	}
}

std::optional<GameLogic::PieceIt> GameLogic::getPiece(Point relPos)
{
	for(auto it = pieces.begin(); it != pieces.end(); ++it) {
		if((*it)->getRelPos() == relPos) {
			return it;
		}
	}

	return std::nullopt;
}

Logic::Cell &GameLogic::getCell(Point relPos)
{
	return *cells.at(relPos.x).at(relPos.y);
}

std::list<Logic::Point> GameLogic::getPosBetween(Point startPos, Point endPos)
{
	if(startPos == endPos)
		return {};
	std::list<Point> output;
	if(startPos.x == endPos.x) { // vertical line
		int minY = startPos.y, maxY = endPos.y;
		if(minY > maxY)
			std::swap(minY, maxY);
		for(int y = minY + 1; y < maxY; ++y) {
			output.push_back(Point(startPos.x, y));
		}
	}
	else if(startPos.y == endPos.y) { // horizontal line
		int minX = startPos.x, maxX = endPos.x;
		if(minX > maxX)
			std::swap(minX, maxX);
		for(int x = minX + 1; x < maxX; ++x) {
			output.push_back(Point(x, startPos.y));
		}
	}
	else if(std::abs(startPos.x - endPos.x) == std::abs(startPos.y - endPos.y)) { // diagonal
		const int xDelta = (endPos.x - startPos.x > 0 ? 1 : -1); // xDelta = 1, we move right, otherwise left
		const int yDelta = (endPos.y - startPos.y > 0 ? 1 : -1); // yDelta = 1, we move up, otherwise down
		for(int x = startPos.x + xDelta, y = startPos.y + yDelta; x != endPos.x; x += xDelta, y += yDelta) {
			output.push_back(Point(x, y));
		}
	}

	return output;
}

bool GameLogic::areCellsNotOccupied(std::list<Point> cells)
{
	for(auto &cellPos: cells) {
		if(getCell(cellPos).getIsOccupied())
			return false;
	}

	return true;
}

std::vector<Logic::Point> GameLogic::getCellAttackers(Point cellPos, PlayerColor defenderPlayerColor)
{
	std::vector<Logic::Point> result;
	for(auto &piece: pieces) {
		if(piece->color != defenderPlayerColor && piece->getRelPos() != cellPos && piece->canCapture(cellPos) && areCellsNotOccupied(getPosBetween(cellPos, piece->getRelPos()))) {
			result.push_back(piece->getRelPos());
		}
	}
	return result;
}

std::vector<Logic::Point> GameLogic::getCellAttackersWithChange(Point cellPos, PlayerColor defenderPlayerColor, Point changedPieceOldPos, Point changedPieceNewPos)
{
	Cell &cellWithChange = getCell(changedPieceNewPos), &oldCell = getCell(changedPieceOldPos);

	const bool isChangedCellOccupied = cellWithChange.getIsOccupied();
	cellWithChange.occupy();
	oldCell.setFree();

	std::vector<Logic::Point> result;
	for(auto &piece: pieces) {
		if(piece->color != defenderPlayerColor &&
				piece->canCapture(cellPos) && areCellsNotOccupied(getPosBetween(cellPos, piece->getRelPos()))) {
			result.push_back(piece->getRelPos());
		}
	}
	if(!isChangedCellOccupied)
		cellWithChange.setFree();
	oldCell.occupy();

	return result;
}

Logic::Point GameLogic::getKingPos(PlayerColor playerColor)
{
	for(auto &piece: pieces) {
		if(piece->color == playerColor && checkPieceType<King>(*piece)) {
			return piece->getRelPos();
		}
	}

	throw std::logic_error("There is no " + PlayerColorToString(playerColor) + " king");
}

void GameLogic::switchTurn()
{
	currentTurn = getInvertedPlayerColor(currentTurn);
}

bool GameLogic::isMovePossible(const Piece &piece, Point newRelPos)
{
	if(piece.color != currentTurn)
		return false;

	const Point oldRelPos = piece.getRelPos();
	if(!areCellsNotOccupied(getPosBetween(oldRelPos, newRelPos)))
		return false;

	const Cell &newCell = getCell(newRelPos);
	if(!newCell.getIsOccupied() && piece.canMakeMove(newRelPos)) {
		// If the king moves then it is enough to check if newRelPos isn't in check
		if(checkPieceType<King>(piece))
			return getCellAttackersWithChange(newRelPos, currentTurn, piece.getRelPos(), newRelPos).empty();
		// If the king doesn't move, then we need to check if the player's king isn't in check after the move
		else
			return getCellAttackersWithChange(getKingPos(currentTurn), currentTurn, piece.getRelPos(), newRelPos).empty();
	}
	return false;
}

bool GameLogic::isCapturePossible(const Piece &piece, Point newRelPos)
{
	if(piece.color != currentTurn)
		return false;

	const Point oldRelPos = piece.getRelPos();
	if(!areCellsNotOccupied(getPosBetween(oldRelPos, newRelPos)))
		return false;

	const Cell &newCell = getCell(newRelPos);
	if(newCell.getIsOccupied()) {
		const auto anotherPiece = getPiece(newRelPos).value();
		if((*anotherPiece)->color != piece.color && piece.canCapture(newRelPos)) {
			if(checkPieceType<King>(piece)) {
				const auto kingCellAttackers = getCellAttackers(newRelPos, piece.color);
				return kingCellAttackers.empty();
			}
			else {
				// If the player's king is still in check after the move then this move is illegal
				const auto kingCellAttackers = getCellAttackersWithChange(getKingPos(currentTurn), currentTurn, oldRelPos, newRelPos);
				// if a piece is to capture some piece than that piece can't capture tha attacker
				return kingCellAttackers.empty() || (kingCellAttackers.size() == 1 && kingCellAttackers.back() == newRelPos);
			}
		}
	}

	return false;
}

std::optional<std::pair<Logic::Point, Logic::Point>> GameLogic::isCastlePossible(const Piece &piece, Point newRelPos)
{
	if(piece.color != currentTurn)
		return std::nullopt;

	const Point oldRelPos = piece.getRelPos();
	if(!areCellsNotOccupied(getPosBetween(oldRelPos, newRelPos)))
		return std::nullopt;

	const auto anotherPiece = getPiece(newRelPos);
	if(anotherPiece.has_value() && checkPieceType<King>(piece) && checkPieceType<Rook>(**anotherPiece.value()) &&
			(*anotherPiece.value())->color == piece.color) {
		// if the king is in the check he can't castle
		if(!getCellAttackers(oldRelPos, piece.color).empty()) {
			return std::nullopt;
		}
		const auto newKingPos = piece.canCastle(newRelPos), newRookPos = (*anotherPiece.value())->canCastle(oldRelPos);
		if(newKingPos.has_value() && newRookPos.has_value()) {
			// if any cells in the way of the king is checked then he can't castle
			const auto posBetweenKingOldAndNewPos = getPosBetween(oldRelPos, newKingPos.value());
			for(auto pos: posBetweenKingOldAndNewPos) {
				if(!getCellAttackers(pos, piece.color).empty()) {
					return std::nullopt;
				}

			}
			return std::make_pair(newKingPos.value(), newRookPos.value());
		}
	}

	return std::nullopt;
}

bool GameLogic::checkForCheckmate()
{
	if(gameStatus != GameStatus::ACTIVE)
		return false;

	const PieceIt king = getPiece(getKingPos(currentTurn)).value();
	// if the king isn't in check, so there is no checkmate
	const auto cellAtackers = getCellAttackers((*king)->getRelPos(), (*king)->color);
	if(cellAtackers.empty())
		return false;

	for(int x = 0; x < Config::CELLS_IN_THE_ROW; ++x) {
		for(int y = 0; y < Config::CELLS_IN_THE_ROW; ++y) {
			if(isMovePossible(**king, Point(x, y)))
				return false;
		}
	}

	// When there are more then two attackers, the only way is to move the king
	if(cellAtackers.size() > 1)
		return false;

	for(const auto &piece: pieces) {
		// sometimes the king can capture the attacker
		if(isCapturePossible(*piece, cellAtackers[0]))
			return false;
		for(int x = 0; x < Config::CELLS_IN_THE_ROW; ++x) {
			for(int y = 0; y < Config::CELLS_IN_THE_ROW; ++y) {
				// if some player's piece can castle the attacker, then there is no checkamte
				if(!checkPieceType<King>(*piece) && piece->color == currentTurn &&
						getCellAttackersWithChange((*king)->getRelPos(), currentTurn, piece->getRelPos(), Point(x, y)).empty())
					return false;
			}
		}
	}

	gameStatus = GameStatus::CHECKMATE;
	return true;
}

bool GameLogic::checkForStalemate()
{
	if(gameStatus != GameStatus::ACTIVE)
		return false;

	// when no pieces can move and the king isn't in check then it's stalemate
	if(!getCellAttackers(getKingPos(currentTurn), currentTurn).empty())
		return false;

	for(const auto &piece: pieces) {
		if(piece->color == currentTurn) {
			for(int x = 0; x < Config::CELLS_IN_THE_ROW; ++x) {
				for(int y = 0; y < Config::CELLS_IN_THE_ROW; ++y) {
					if(isMovePossible(*piece, Point(x, y)) || isCapturePossible(*piece, Point(x, y))) {
						return false;
					}
				}
			}
		}
	}

	gameStatus = GameStatus::STALEMATE;
	return true;
}
