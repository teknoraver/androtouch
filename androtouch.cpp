#include <QMessageBox>
#include <QPixmap>
#include <QMouseEvent>
#include <QProcess>

#include "androtouch.h"

AndroTouch::AndroTouch() : QMainWindow(0)
{
	setupUi(this);
	connect(actionAbout, SIGNAL(triggered()), SLOT(about()));
	connect(actionAbout_Qt, SIGNAL(triggered()), QCoreApplication::instance(), SLOT(aboutQt()));
	connect(&grabber, SIGNAL(grabbed(QByteArray*)), SLOT(sshot(QByteArray*)));
	connect(screen, SIGNAL(clicked(QMouseEvent*)), SLOT(touch(QMouseEvent*)));
	connect(screen, SIGNAL(unclicked(QMouseEvent*)), SLOT(touch(QMouseEvent*)));

	grabber.start();
}

void AndroTouch::about()
{
        QMessageBox::about(this, "About AndroTouch", "AndroTouch - a Qt AndroTouch application<br>by Matteo Croce <a href=\"http://teknoraver.net/\">http://teknoraver.net/</a>");
}

void Grabber::run()
{
	while(1) {
		QProcess adb;
		adb.start("adb", QStringList() << "shell" << "screencap" << "-p");
		adb.waitForFinished(-1);
		QByteArray bytes = adb.readAllStandardOutput().replace("\r\n", "\n");

		if(!bytes.startsWith("\x89PNG\r\n\x1a\n")) {
			qDebug("invalid image, missing PNG");
			continue;
		}
		if(!bytes.endsWith("IEND\xae\x42\x60\x82")) {
			qDebug("invalid image, missing IEND");
			continue;
		}
		//qDebug("size: %d", bytes.size());
		png = bytes;
		emit grabbed(&png);
	}
}

void AndroTouch::sshot(QByteArray *bytes)
{
	QPixmap png;
	png.loadFromData(*bytes, "PNG");
	swidth = png.width();
	sheight = png.height();
	png = png.scaledToHeight(screen->height(), Qt::SmoothTransformation);
	screen->setPixmap(png);
}

void AndroTouch::touch(QMouseEvent *evt)
{
	int x = evt->x() * swidth / screen->width();
	int y = evt->y() * sheight / screen->height();
	//qDebug("touch: %d, %d, %d", x, y, evt->type());

	if(evt->type() == QEvent::MouseButtonPress) {
		lastx = x;
		lasty = y;
		return;
	}

	QStringList args;
	args  << "shell" << "input";

	if(x == lastx && y == lasty)
		args << "tap";
	else
		args << "swipe" << QString::number(lastx) << QString::number(lasty);

	QProcess::execute("adb", args << QString::number(x) << QString::number(y));
}
