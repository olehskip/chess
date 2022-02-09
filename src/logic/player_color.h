#ifndef LOGIC_PLAYER_COLOR_H
#define LOGIC_PLAYER_COLOR_H

#include <string>

namespace Logic
{
	enum class PlayerColor
	{
		WHITE,
		BLACK
	};

	PlayerColor getInvertedPlayerColor(PlayerColor playerColor);
	std::string PlayerColorToString(PlayerColor playerColor);
}

#endif // LOGIC_PLAYER_COLOR_H
