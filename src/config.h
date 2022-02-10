#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QColor>

namespace Config
{
	const int CELLS_IN_THE_ROW = 8;

	const static QColor cellBlackColor = QColor("#4b7399"), cellWhiteColor = QColor("#eae9d2");
	namespace PIECE_PATHS
	{
		const QString WHITE_BISHOP = ":/white_pieces/bishop.png";
		const QString WHITE_KING = ":/white_pieces/king.png";
		const QString WHITE_KNIGHT = ":/white_pieces/knight.png";
		const QString WHITE_PAWN = ":/white_pieces/pawn.png";
		const QString WHITE_QUEEN = ":/white_pieces/queen.png";
		const QString WHITE_ROOK = ":/white_pieces/rook.png";

		const QString BLACK_BISHOP = ":/black_pieces/bishop.png";
		const QString BLACK_KING = ":/black_pieces/king.png";
		const QString BLACK_KNIGHT = ":/black_pieces/knight.png";
		const QString BLACK_PAWN = ":/black_pieces/pawn.png";
		const QString BLACK_QUEEN = ":/black_pieces/queen.png";
		const QString BLACK_ROOK = ":/black_pieces/rook.png";
	}
}

#endif // CONFIG_H
