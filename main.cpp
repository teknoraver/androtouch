#include <QApplication>
#include <QDesktopWidget>
#include "androtouch.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	AndroTouch app;
	app.move((QApplication::desktop()->screenGeometry(&app).width() - app.width()) / 2, (QApplication::desktop()->screenGeometry(&app).height() - app.height()) / 2);
	app.setVisible(true);
	return a.exec();
}
