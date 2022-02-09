#include "windows/game_window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	Windows::GameWindow w;
	w.show();
	return a.exec();
}
