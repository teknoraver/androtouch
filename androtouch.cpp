#include <QMessageBox>
#include <QPixmap>
#include <QMouseEvent>
#include <QProcess>

#ifdef QT_DEBUG
#include <QLoggingCategory>
#endif

#include "androtouch.h"

AndroTouch::AndroTouch() : QMainWindow(0), grabber(this)
{
#ifdef QT_DEBUG
	QLoggingCategory::defaultCategory()->setEnabled(QtDebugMsg, true);
	qDebug("Init");
#endif

	setupUi(this);
	connect(actionAbout, SIGNAL(triggered()), SLOT(about()));
	connect(actionAbout_Qt, SIGNAL(triggered()), QCoreApplication::instance(), SLOT(aboutQt()));
	connect(&grabber, SIGNAL(grabbed(QByteArray*)), SLOT(sshot(QByteArray*)));
	connect(phoneScreen, SIGNAL(clicked(QMouseEvent*)), SLOT(touch(QMouseEvent*)));
	connect(phoneScreen, SIGNAL(unclicked(QMouseEvent*)), SLOT(touch(QMouseEvent*)));

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
		QByteArray bytes = adb.readAllStandardOutput();

		if (ui->actionCR->isChecked())
			bytes = adb.readAllStandardOutput().replace("\r\n", "\n");

		if(!bytes.startsWith("\x89PNG\r\n\x1a\n")) {
			ui->statusbar->showMessage("invalid image, missing PNG");
			continue;
		}
		if(!bytes.endsWith("IEND\xae\x42\x60\x82")) {
			ui->statusbar->showMessage("invalid image, missing IEND");
			continue;
		}
#ifdef QT_DEBUG
		qDebug() << "size" << bytes.size();
#endif
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
	png = png.scaledToHeight(phoneScreen->height(), Qt::SmoothTransformation);
	phoneScreen->setPixmap(png);
}

void AndroTouch::touch(QMouseEvent *evt)
{
	int x = evt->x() * swidth / phoneScreen->width();
	int y = evt->y() * sheight / phoneScreen->height();
#ifdef QT_DEBUG
	qDebug() << "touch:" << x << y << evt->type();
#endif

	if(evt->type() == QEvent::MouseButtonPress) {
		lastx = x;
		lasty = y;
		return;
	}

	QStringList args("shell");

	if (actionSU->isChecked())
		args  << "su" << "-c";

	args  << "input";

	if(x == lastx && y == lasty)
		args << "tap";
	else
		args << "swipe" << QString::number(lastx) << QString::number(lasty);

#ifdef QT_DEBUG
	qDebug() << "input:" << args;
#endif

	QProcess::execute("adb", args << QString::number(x) << QString::number(y));
}
