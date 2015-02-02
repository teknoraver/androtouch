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
	connect(&grabber, SIGNAL(grabbed(QByteArray*)), SLOT(sshot(QByteArray*)));
	connect(screen, SIGNAL(clicked(QMouseEvent*)), SLOT(touch(QMouseEvent*)));

	grabber.start();
}

void AndroTouch::about()
{
        QMessageBox::about(this, "About AndroTouch", "AndroTouch - a Qt AndroTouch application<br>by Matteo Croce <a href=\"http://teknoraver.net/\">http://teknoraver.net/</a>");
}

Grabber::Grabber()
{
	adb.start("adb shell");
	adb.write("PS1=\n");
}

void Grabber::run()
{
	while(1) {
		adb.write("screencap -p\n");
		adb.waitForBytesWritten(-1);
		QByteArray bytes;
		while(!bytes.contains("IEND\xae\x42\x60\x82")) {
			bytes += adb.readAll();
			msleep(100);
		}
		bytes = bytes.replace("\r\n", "\n");
		int chunk = bytes.indexOf("\x89PNG\r\n\x1a\n");
		if(chunk == -1) {
			//////
			QFile file("nopng.png");
			file.open(QIODevice::ReadWrite);
			file.write(bytes);
			file.close();
			/////////
			qDebug("invalid image, missing PNG");
			continue;
		}
		bytes = bytes.remove(0, chunk);
		chunk = bytes.indexOf("IEND\xae\x42\x60\x82");
		if(chunk == -1) {
			//////
			QFile file("noiend.png");
			file.open(QIODevice::ReadWrite);
			file.write(bytes);
			file.close();
			/////////
			qDebug("invalid image, missing IEND");
			continue;
		}
		bytes.truncate(chunk + 8);
		qDebug("size: %d", bytes.size());
//////
QFile file("sshot.png");
file.open(QIODevice::ReadWrite);
file.write(bytes);
file.close();
/////////
		png = bytes;
		emit grabbed(&png);
	}
}

void AndroTouch::sshot(QByteArray *bytes)
{
	QPixmap png;
	png.loadFromData(*bytes, "PNG");
	png = png.scaledToHeight(screen->height(), Qt::SmoothTransformation);
	screen->setPixmap(png);
}

void AndroTouch::touch(QMouseEvent *evt)
{
	qDebug("touch");
}
