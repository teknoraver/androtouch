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
	int x = evt->x() * swidth / phoneScreen->pixmap()->width();
	int y = evt->y() * sheight / phoneScreen->pixmap()->height();
#ifdef QT_DEBUG
	qDebug() << "touch:" << x << y << evt->type() << evt->button();
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

	if(x == lastx && y == lasty) {
		switch (evt->button()) {
		case Qt::LeftButton:
			args << "tap" << QString::number(lastx) << QString::number(lasty);
			break;
		case Qt::RightButton:
			args << "draganddrop" <<
				QString::number(lastx) << QString::number(lasty) <<
				QString::number(lastx) << QString::number(lasty) <<
				"1050";
			break;
		default:
			return;
		}
	} else {
		args << "swipe" <<
			QString::number(lastx) << QString::number(lasty) <<
			QString::number(lastx) << QString::number(lasty);
	}

#ifdef QT_DEBUG
	qDebug() << "input:" << args;
#endif

	QProcess::execute("adb", args);
}

void AndroTouch::keyPressEvent(QKeyEvent *event)
{
#ifdef QT_DEBUG
	qDebug() << "key:" << event;
#endif

	QStringList args("shell");

	if (actionSU->isChecked())
		args  << "su" << "-c";

	int oldlen = args.count();

	switch (event->key()) {
	case Qt::Key_Left:
		args << "input" << "keyevent" << "KEYCODE_DPAD_LEFT";
		break;
	case Qt::Key_Up:
		args << "input" << "keyevent" << "KEYCODE_DPAD_UP";
		break;
	case Qt::Key_Right:
		args << "input" << "keyevent" << "KEYCODE_DPAD_RIGHT";
		break;
	case Qt::Key_Down:
		args << "input" << "keyevent" << "KEYCODE_DPAD_DOWN";
		break;
	case Qt::Key_Return:
		args << "input" << "keyevent" << "KEYCODE_ENTER";
		break;
	case Qt::Key_Backspace:
		args << "input" << "keyevent" << "KEYCODE_DEL";
		break;
	case Qt::Key_Escape:
		args << "input" << "keyevent" << "KEYCODE_BACK";
		break;
	case Qt::Key_Space:
		args << "input" << "keyevent" << "KEYCODE_SPACE";
		break;
	case Qt::Key_Semicolon:
		args << "input" << "keyevent" << "KEYCODE_SEMICOLON";
		break;
	case Qt::Key_ParenLeft:
		args << "input" << "keyevent" << "KEYCODE_NUMPAD_LEFT_PAREN";
		break;
	case Qt::Key_ParenRight:
		args << "input" << "keyevent" << "KEYCODE_NUMPAD_RIGHT_PAREN";
		break;
	case Qt::Key_Asterisk:
		args << "input" << "keyevent" << "KEYCODE_STAR";
		break;
	case Qt::Key_QuoteLeft:
		args << "input" << "keyevent" << "KEYCODE_GRAVE";
		break;
	case Qt::Key_NumberSign:
		args << "input" << "keyevent" << "KEYCODE_POUND";
		break;
	case Qt::Key_CapsLock:
		args << "input" << "keyevent" << "KEYCODE_CAPS_LOCK";
		break;
	case Qt::Key_Home:
		args << "input" << "keyevent" << "KEYCODE_MOVE_HOME";
		break;
	case Qt::Key_End:
		args << "input" << "keyevent" << "KEYCODE_MOVE_END";
		break;
	}

	if (args.count() == oldlen) {
		if (!event->text().size())
			return;
		args << "input" << "text" << ('\'' + event->text() + '\'');
	}

#ifdef QT_DEBUG
	qDebug() << "key:" << args;
#endif

	QProcess::execute("adb", args);
}
