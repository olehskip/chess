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

	void addNotation(QString notationText);
	void addEndgameNotification(QString text);
	QTableWidgetItem *createItem(QString text);

signals:
	void newGameTriggered();

private:
	QGridLayout mainLayout;
	QTableWidget tableWidget;
	QPushButton newGameButton;

	bool currentColumnIndex = 0;

	void newGame();
};

#endif
