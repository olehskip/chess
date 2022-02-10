#ifndef SIDE_PANEL_H
#define SIDE_PANEL_H

#include <QWidget>
#include <QGridLayout>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QTableWidgetItem>


namespace Widgets
{
	class SidePanel;
};

class Widgets::SidePanel: public QWidget
{
	Q_OBJECT

public:
	SidePanel();

	void addNotation(QString notationText); // a move was made
	void addEndgameNotification(QString text); // checkmate or stalemate

signals:
	void newGameTriggered(); // new game button is triggered

private:
	QGridLayout mainLayout;
	QTableWidget tableWidget;
	QPushButton newGameButton;

	bool currentColumnIndex = 0;

	/*
	 * Returns a pointer to a QTableWidgetItem,
	 * we need this function in order to make all the items look the same
	 */
	QTableWidgetItem *createItem(QString text);
	void newGame();
};

#endif
