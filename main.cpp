#include <QApplication>
#include "androtouch.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	AndroTouch app;
	app.show();
	return a.exec();
}
