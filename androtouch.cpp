#include <unistd.h>
#include <QMessageBox>
#include <QTimer>
#include <QFile>
#include <QPixmap>

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
	QByteArray bytes;
	while(adb.bytesAvailable() != 0) {
		bytes += adb.readAll();
		usleep(100 * 1000);
	}

	bytes.remove(0, strlen("screencap -p\r\n"));
	bytes = bytes.replace("\r\n", "\n");
	qDebug("size: %d", bytes.size());
	//////
	QFile file("shot.png");
	file.open(QIODevice::ReadWrite);
	file.write(bytes);
	file.close();
	/////////
	QPixmap png;
	png.loadFromData(bytes, "PNG");
//	png.loadFromData(adb.readAll().replace("\r\n", "\n"), "PNG");
	screen->setPixmap(png);

	QTimer::singleShot(1000, this, SLOT(sshot()));
}
