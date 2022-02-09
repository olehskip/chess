#ifndef LOGIC_GAME_LOGIC_H
#define LOGIC_GAME_LOGIC_H

#include <list>
#include <memory>
#include <string>

#include "player_color.h"
#include "piece.h"
#include "cell.h"

namespace Logic
{
	class GameLogic;

	enum class GameStatus
	{
		ACTIVE,
		CHECKMATE,
		STALEMATE,
		PROMOTION_REQUIRED // during this status the players can't move
	};

	struct MoveResponse
	{
		bool isMoveApproved = false, isPieceWasCaptured = false;
		std::string notationText;
		std::vector<std::pair<Point /* oldPos */, Point /* newPos */>> posChanges;
	};
}

class Logic::GameLogic
{
public:
	GameLogic();

	void startNewGame();

	MoveResponse makeMove(Point oldRelPos, Point newRelPos);
	template<typename T>
	bool makePromotion()
	{
		if(!std::is_same_v<T, Queen> && !std::is_same_v<T, Rook> && !std::is_same_v<T, Bishop> && !std::is_same_v<T, Knight>)
			return false;

		if(gameStatus != GameStatus::PROMOTION_REQUIRED || !promotionRelPos.has_value())
			return false;

		auto piece = getPiece(promotionRelPos.value());
		if(piece.has_value() && checkPieceType<Pawn>(**piece.value())) {
			const auto color = (*piece.value())->color;
			pieces.erase(piece.value());
			pieces.emplace_back(new T(color, promotionRelPos.value()));
			gameStatus = GameStatus::ACTIVE;
			promotionRelPos = std::nullopt;
			checkForCheckmate();
			checkForStalemate();
			return true;
		}

		return false;
	}

	GameStatus getGameStatus() const;

private:
	PlayerColor currentTurn = PlayerColor::WHITE;
	GameStatus gameStatus = GameStatus::ACTIVE;
	std::optional<Point> promotionRelPos = std::nullopt;

	std::vector<std::vector<std::unique_ptr<Cell>>> cells;
	std::list<std::unique_ptr<Piece>> pieces;
	typedef decltype(pieces)::iterator PieceIt;
	void fillPieces();
	void fillCells();

	std::optional<PieceIt> getPiece(Point relPos); // returns a piece with relPos, if there is no such piece then an exception is thrown
	Cell &getCell(Point relPos);  // returns a cell with relPos, if there is no such cell then an exception is thrown

	/*
	 * Returns position of all the cells that lie on the *line between startPos and ensPos(startPos and endPos are not included)
	 * we need this function, because all the pieces(except the knight) can't jump over other pieces
	 * line can be horizontal, vertical or diagonal, otherwise nothing is returned
	*/
	std::list<Point> getPosBetween(Point startPos, Point endPos);
	bool areCellsNotOccupied(std::list<Point> cells);

	/*
	 * Returns a piece that can capture a piece located at cellPos, that piece should have a difference color than defenderPlayer
	 * possibly no pieces are located at cellPos, in other words the function check if the cell is in check
	 * if there is no attacker std::nullopt is returned
	 */
	std::vector<Point> getCellAttackers(Point cellPos, PlayerColor defenderPlayerColor);

	/*
	 * This function is similliar to getCellAttacker, but this one "imagines" that piece is located at newPiecePos
	 * this function exists because we need to check if the king is not in check after moving some piece
	 * defenderPlayerColor argument is not used because the function uses the piece color as defenderPlayerColor
	 */
	std::vector<Point> getCellAttackersWithChange(Point cellPos, PlayerColor defenderPlayerColor, Point changedPieceOldPos, Point changedPieceNewPos);


	template<class T>
	bool checkPieceType(const Piece &piece)
	{
		return dynamic_cast<const T*>(&piece) != nullptr;
	}
	Point getKingPos(PlayerColor playerColor);

	void switchTurn();

	bool isMovePossible(const Piece &piece, Point newRelPos);
	bool isCapturePossible(const Piece &piece, Point newRelPos);
	std::optional<std::pair<Point, Point>> isCastlePossible(const Piece &piece, Point newRelPos);

	bool checkForCheckmate();
	bool checkForStalemate();
};

#endif // LOGIC_GAME_LOGIC_H
