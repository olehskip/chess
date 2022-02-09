#include "side_panel.h"

using Widgets::SidePanel;

SidePanel::SidePanel()
{
	setStyleSheet("border: 3px solid white");
	mainLayout.addWidget(&tableWidget, 0, 0);

	tableWidget.setColumnCount(2);
	tableWidget.setItem(0, 0, new QTableWidgetItem("White"));
	tableWidget.setHorizontalHeaderLabels(QStringList({"White", "Black"}));
	tableWidget.horizontalHeader()->setStyleSheet("border: none;");
	tableWidget.horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	tableWidget.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	tableWidget.verticalHeader()->hide();
	tableWidget.setSelectionMode(QAbstractItemView::NoSelection);
	tableWidget.setEditTriggers(QAbstractItemView::NoEditTriggers);
	tableWidget.setStyleSheet("QTableWidget{border: 2px solid white;} QTableWidget::item,QHeaderView::section {color: white;}");

	newGameButton.setText("New game");
	connect(&newGameButton, &QPushButton::clicked, this, &SidePanel::newGame);
	mainLayout.addWidget(&newGameButton, 1, 0);

	setLayout(&mainLayout);
	setFocusPolicy(Qt::NoFocus);
	tableWidget.setFocusPolicy(Qt::NoFocus);
	newGameButton.setFocusPolicy(Qt::NoFocus);
	newGameButton.setMinimumHeight(35);
}

void SidePanel::addNotation(QString notationText)
{
	if(currentColumnIndex == 0)
		tableWidget.insertRow(tableWidget.rowCount());

	tableWidget.setItem(tableWidget.rowCount() - 1, currentColumnIndex, createItem(notationText));
	currentColumnIndex = !currentColumnIndex;
}

void SidePanel::addEndgameNotification(QString text)
{
	tableWidget.insertRow(tableWidget.rowCount());
	tableWidget.setSpan(tableWidget.rowCount() - 1, 0, 1, 2);
	tableWidget.setItem(tableWidget.rowCount() - 1, 0, createItem(text));
}

QTableWidgetItem *SidePanel::createItem(QString text)
{
	QTableWidgetItem *item = new QTableWidgetItem(text);
	item->setTextAlignment(Qt::AlignCenter);
	return item;
}

void SidePanel::newGame()
{
	currentColumnIndex = 0;
	tableWidget.setRowCount(0);
	emit newGameTriggered();
}
