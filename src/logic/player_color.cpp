#include "player_color.h"

Logic::PlayerColor Logic::getInvertedPlayerColor(PlayerColor playerColor)
{
	return (playerColor == PlayerColor::WHITE ? PlayerColor::BLACK : PlayerColor::WHITE);
}

std::string Logic::PlayerColorToString(PlayerColor playerColor)
{
	return (playerColor == PlayerColor::WHITE ? "White" : "Black");
}
