#include "game_window.h"

using Windows::GameWindow;
GameWindow::GameWindow()
{
	setMinimumSize(700, 450);
	mainLayout.setSpacing(0);
	mainLayout.addWidget(&board, 20);
	mainLayout.addWidget(&sidePanel, 1);
	sidePanel.setMinimumWidth(200);
	setLayout(&mainLayout);

	connect(&sidePanel, &Widgets::SidePanel::newGameTriggered, this, &GameWindow::startNewGame);
	connect(&board, &Widgets::Board::moveWasMade, this, &GameWindow::processMakeMove);

	setFocusPolicy(Qt::NoFocus);
}

void GameWindow::resizeEvent(QResizeEvent *event)
{
	// the cells should be squares, so should the board
	if(event->size().width() - sidePanel.minimumWidth() > event->size().height()) {
		board.setMinimumHeight(400);
		board.setMaximumHeight(16777215);
		board.setFixedWidth(board.height());
	}
	else {
		board.setMinimumWidth(400);
		board.setMaximumWidth(16777215);
		board.setFixedHeight(board.width());
	}
	QWidget::resizeEvent(event);
}

void GameWindow::processMakeMove(QPoint pieceOriginalRelPos, QPoint pieceNewRelPos)
{
	const Logic::Point oldPos = fromQPointToLogicPoint(pieceOriginalRelPos), newPos = fromQPointToLogicPoint(pieceNewRelPos);
	const Logic::MoveResponse moveResponse = gameLogic.makeMove(oldPos, newPos);
	if(moveResponse.isMoveApproved) {
		if(moveResponse.isPieceWasCaptured)
			board.removePiece(pieceNewRelPos);

		for(auto posChange: moveResponse.posChanges)
			board.acceptMove(fromLogicPointToQPoint(posChange.first), fromLogicPointToQPoint(posChange.second));

		if(!moveResponse.notationText.empty())
			sidePanel.addNotation(QString::fromStdString(moveResponse.notationText));
		const auto gameStatus = gameLogic.getGameStatus();
		if(gameStatus == Logic::GameStatus::PROMOTION_REQUIRED) {
			board.makePromotion(pieceNewRelPos);
			connect(&board, &Widgets::Board::promotionWasMade, this, &GameWindow::processPromotion);
		}
		checkForGameEnd();
	}
	else {
		board.putPieceBack(pieceOriginalRelPos);
	}
}

void GameWindow::processPromotion(SelectedPieceType selectedPieceType)
{
	disconnect(&board, &Widgets::Board::promotionWasMade, this, &GameWindow::processPromotion);
	if(selectedPieceType == SelectedPieceType::QUEEN)
		gameLogic.makePromotion<Logic::Queen>();
	else if(selectedPieceType == SelectedPieceType::ROOK)
		gameLogic.makePromotion<Logic::Rook>();
	else if(selectedPieceType == SelectedPieceType::BISHOP)
		gameLogic.makePromotion<Logic::Bishop>();
	else if(selectedPieceType == SelectedPieceType::KNIGHT)
		gameLogic.makePromotion<Logic::Knight>();

	checkForGameEnd();
}

QPoint GameWindow::fromLogicPointToQPoint(Logic::Point point) const
{
	return QPoint(point.x, point.y);
}

Logic::Point GameWindow::fromQPointToLogicPoint(QPoint point) const
{
	return Logic::Point(point.x(), point.y());
}

void GameWindow::startNewGame()
{
	board.startNewGame();
	gameLogic.startNewGame();
}

void GameWindow::checkForGameEnd()
{
	const auto gameStatus = gameLogic.getGameStatus();
	if(gameStatus == Logic::GameStatus::CHECKMATE)
		sidePanel.addEndgameNotification("Checkmate");
	else if(gameStatus == Logic::GameStatus::STALEMATE)
		sidePanel.addEndgameNotification("Stalemate");
}
