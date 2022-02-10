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

	/*
	 * T - a class to which promote
	 * there are no arguments because the class memorizes which piece to promote
	 * this function is used when the player decides what piece type he wants to promote the piece to
	 */
	template<typename T>
	bool makePromotion();

	GameStatus getGameStatus() const;

private:
	PlayerColor currentTurn = PlayerColor::WHITE;
	GameStatus gameStatus = GameStatus::ACTIVE;

	/*
	 * When a pawn reaches the end of the board, then the player should promote the piece,
	 * but Board class is responsible for the offering promotion(futhermore the player thinking can take a bit)
	 * so we need to memorize what piece to promote
	 */
	std::optional<Point> promotionRelPos = std::nullopt;

	std::vector<std::vector<std::unique_ptr<Cell>>> cells;
	std::list<std::unique_ptr<Piece>> pieces;
	typedef decltype(pieces)::iterator PieceIt;
	void fillPieces();
	void fillCells();

	std::optional<PieceIt> getPiece(Point relPos); // returns a piece with relPos
	Cell &getCell(Point relPos);  // returns a cell with relPos, if there is no such cell then an exception is thrown

	/*
	 * Returns position of all the cells that lie on the *line between startPos and endPos(startPos and endPos are not included)
	 * we need this function, because all the pieces(except the knight) can't jump over other pieces
	 * *line can be horizontal, vertical or diagonal, otherwise an empty list is returned
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
	bool checkPieceType(const Piece &piece);
	Point getKingPos(PlayerColor playerColor);

	void switchTurn();

	bool isMovePossible(const Piece &piece, Point newRelPos);
	bool isCapturePossible(const Piece &piece, Point newRelPos);
	std::optional<std::pair<Point, Point>> isCastlePossible(const Piece &piece, Point newRelPos);

	bool checkForCheckmate();
	bool checkForStalemate();
};

#include "game_logic.tpp"

#endif // LOGIC_GAME_LOGIC_H
