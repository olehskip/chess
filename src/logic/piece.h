#ifndef LOGIC_PIECE_H
#define LOGIC_PIECE_H

#include <optional>

#include "player_color.h"
#include "point.h"

namespace Logic
{
	class Piece;
	class Pawn;
	class Bishop;
	class Knight;
	class Rook;
	class Queen;
	class King;
}

class Logic::Piece
{
public:

	// this function should check only if move is legal(imagining that there are no other pieces on the board)
	virtual bool canMakeMove(Point newRelPos) const = 0;
	// Most of pieces capture the same way as they make moves, but for example the pawn does in a different way
	virtual bool canCapture(Point newRelPos) const;
	virtual std::optional<Point> canCastle(Point newRelPos) const;
	virtual bool canPromote(Point newRelPos) const;
	Point getRelPos() const;
	void setRelPos(Point newRelPos);

	const PlayerColor color;

	int movesCount = 0; // this variable used for the rook and the king, because they can castle only when they didn't move
	const char notationSymbol;
protected:
	Piece(PlayerColor color_, Point initialRelPos_, char notationSymbol_);

	const Point initialRelPos;
	bool didMove() const;

private:
	Point oldRelPos;
	Point relPos;
};

class Logic::Pawn: public Logic::Piece
{
public:
	Pawn(PlayerColor color_, Point initialRelPos_);
	bool canMakeMove(Point newRelPos) const override;
	bool canCapture(Point newRelPos) const override;
	bool canPromote(Point newRelPos) const override;
};


class Logic::Bishop: public Logic::Piece
{
public:
	Bishop(PlayerColor color_, Point initialRelPos_);
	bool canMakeMove(Point newRelPos) const override;
};


class Logic::Knight: public Logic::Piece
{
public:
	Knight(PlayerColor color_, Point initialRelPos_);
	using Piece::Piece;
	bool canMakeMove(Point newRelPos) const override;
};


class Logic::Rook: public Logic::Piece
{
public:
	Rook(PlayerColor color_, Point initialRelPos_);
	bool canMakeMove(Point) const override;
	std::optional<Point> canCastle(Point newRelPos) const override;
};


class Logic::Queen: public Logic::Piece
{
public:
	Queen(PlayerColor color_, Point initialRelPos_);
	bool canMakeMove(Point newRelPos) const override;
};


class Logic::King: public Logic::Piece
{
public:
	King(PlayerColor color_, Point initialRelPos_);
	bool canMakeMove(Point newRelPos) const override;
	std::optional<Point> canCastle(Point newRelPos) const override;
};

#endif // LOGIC_PIECE_H
