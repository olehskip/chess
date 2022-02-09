#include "piece.h"

using Logic::Piece;
using Logic::Pawn;
using Logic::Bishop;
using Logic::Knight;
using Logic::Rook;
using Logic::Queen;
using Logic::King;

Piece::Piece(PlayerColor color_, Point initialRelPos_, char notationSymbol_): color(color_), notationSymbol(notationSymbol_), initialRelPos(initialRelPos_)
{
	relPos = initialRelPos;
}

bool Piece::canCapture(Point newRelPos) const
{
	return canMakeMove(newRelPos);
}

std::optional<Logic::Point> Piece::canCastle(Point newRelPos) const
{
	return std::nullopt;
}

bool Piece::canPromote(Point newRelPos) const
{
	return false;
}

Logic::Point Piece::getRelPos() const
{
	return relPos;
}

void Piece::setRelPos(Point newRelPos)
{
	oldRelPos = relPos;
	relPos = newRelPos;
	movesCount++;
}

bool Piece::didMove() const
{
	return movesCount != 0;
}

// ---Pawn---
Pawn::Pawn(PlayerColor color_, Point initialRelPos_): Piece(color_, initialRelPos_, '\0')
{

}

bool Pawn::canMakeMove(Point newRelPos) const // override
{
	// Pawn can move only one cell forward or two cells forward if it's on the initial position
	const auto relPos = getRelPos();
	const int diffBetweenNewAndOldY = (color == PlayerColor::WHITE ? 1: -1);
	return (newRelPos.x == relPos.x) && (newRelPos.y - relPos.y == diffBetweenNewAndOldY || (relPos == initialRelPos && newRelPos.y - relPos.y == 2 * diffBetweenNewAndOldY));
}

bool Pawn::canCapture(Point newRelPos) const // override
{
	const int diffBetweenNewAndOldY = (color == PlayerColor::WHITE ? 1: -1);
	const auto relPos = getRelPos();
	return std::abs(newRelPos.x - relPos.x) == 1 && newRelPos.y - relPos.y == diffBetweenNewAndOldY;
}

bool Pawn::canPromote(Point newRelPos) const // override
{
	return (color == PlayerColor::WHITE && newRelPos.y == 7) || (color == PlayerColor::BLACK && newRelPos.y == 0);
}

// ---Bishop---
Bishop::Bishop(PlayerColor color_, Point initialRelPos_): Piece(color_, initialRelPos_, 'B')
{

}

bool Bishop::canMakeMove(Point newRelPos) const // override
{
	// Bishop can move only diagonally
	const auto relPos = getRelPos();
	return std::abs(newRelPos.x - relPos.x) == std::abs(newRelPos.y - relPos.y);
}

// ---Knight---
Knight::Knight(PlayerColor color_, Point initialRelPos_): Piece(color_, initialRelPos_, 'N')
{

}

bool Knight::canMakeMove(Point newRelPos) const // override
{
	const auto relPos = getRelPos();
	const auto xDifference = abs(relPos.x - newRelPos.x), yDifference = abs(relPos.y - newRelPos.y);
	return (xDifference == 1 && yDifference == 2) || (xDifference == 2 && yDifference == 1);
}

// ---Rook---
Rook::Rook(PlayerColor color_, Point initialRelPos_): Piece(color_, initialRelPos_, 'R')
{

}

bool Rook::canMakeMove(Point newRelPos) const // override
{
	// Rook can move horizontally and vertically
	const auto relPos = getRelPos();
	return newRelPos.x == relPos.x || newRelPos.y == relPos.y;
}

std::optional<Logic::Point> Rook::canCastle(Point) const // override
{
	if(didMove())
		return std::nullopt;

	const auto relPos = getRelPos();
	if(relPos.x == 0)
		return Point(3, relPos.y);
	else if(relPos.x == 7)
		return Point(5, relPos.y);

	return std::nullopt;
}

// ---Queen---
Queen::Queen(PlayerColor color_, Point initialRelPos_): Piece(color_, initialRelPos_, 'Q')
{

}

bool Queen::canMakeMove(Point newRelPos) const // override
{
	// Queen can move horizontally, vertically and diagonally
	const auto relPos = getRelPos();
	return newRelPos.x == relPos.x || newRelPos.y == relPos.y || std::abs(newRelPos.x - relPos.x) == std::abs(newRelPos.y - relPos.y);
}

// ---King---
King::King(PlayerColor color_, Point initialRelPos_): Piece(color_, initialRelPos_, 'K')
{

}

bool King::canMakeMove(Point newRelPos) const // override
{
	// King can move only to an adjacent cell
	const auto relPos = getRelPos();
	return abs(relPos.x - newRelPos.x) <= 1 && abs(relPos.y - newRelPos.y) <= 1;
}

std::optional<Logic::Point> King::canCastle(Point newRelPos) const // override
{
	const auto relPos = getRelPos();
	if(didMove() || relPos.y != newRelPos.y || (newRelPos.x != 0 && newRelPos.x != 7))
		return std::nullopt;

	if(newRelPos.x == 0)
		return Point(2, relPos.y);
	else if(newRelPos.x == 7)
		return Point(6, relPos.y);

	return std::nullopt;
}
