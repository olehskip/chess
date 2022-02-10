#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <QWidget>

#include "widgets/board.h"
#include "widgets/side_panel.h"

#include "logic/game_logic.h"


namespace Windows
{
	class GameWindow;
};

class Windows::GameWindow: public QWidget
{
	Q_OBJECT

public:
	GameWindow();

private:
	Widgets::Board board;
	Widgets::SidePanel sidePanel;
	QHBoxLayout mainLayout;
	QWidget spacer;

	void resizeEvent(QResizeEvent *event);

	void processMakeMove(QPoint pieceOriginalRelPos, QPoint pieceNewRelPos);
	void processPromotion(SelectedPieceType selectedPieceType);

	QPoint fromLogicPointToQPoint(Logic::Point point) const;
	Logic::Point fromQPointToLogicPoint(QPoint point) const;

	Logic::GameLogic gameLogic;

	void startNewGame();
	void checkForGameEnd();
};

#endif // GAME_WINDOW_H
