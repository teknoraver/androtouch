#include <unistd.h>
#include <QMessageBox>
#include <QTimer>
#include <QBitmap>

#include "androtouch.h"

AndroTouch::AndroTouch() : QMainWindow(0)
{
	setupUi(this);
	connect(actionAbout, SIGNAL(triggered()), SLOT(about()));
	connect(actionAbout_Qt, SIGNAL(triggered()), QCoreApplication::instance(), SLOT(aboutQt()));

	QTimer::singleShot(1000, this, SLOT(sshot()));

	adb.start("adb shell");
}

AndroTouch::~AndroTouch()
{
}

void AndroTouch::about()
{
        QMessageBox::about(this, "About AndroTouch", "AndroTouch - a Qt AndroTouch application<br>by Matteo Croce <a href=\"http://teknoraver.net/\">http://teknoraver.net/</a>");
}

void AndroTouch::sshot()
{
	adb.write("screencap -p\n");
	adb.waitForBytesWritten(-1);
	sleep(1);
	adb.waitForReadyRead();
	QBitmap png;
	png.loadFromData(adb.readAll().replace("\r\n", "\n"), "PNG");
	screen->setPixmap(png);

	QTimer::singleShot(1000, this, SLOT(sshot()));
}
