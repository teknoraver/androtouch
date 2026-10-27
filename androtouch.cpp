#include <QMessageBox>
#include <QPixmap>
#include <QMouseEvent>
#include <QProcess>

#ifdef QT_DEBUG
#include <QLoggingCategory>
#endif

#include "androtouch.h"

AndroTouch::AndroTouch() : QMainWindow(nullptr)
{
#ifdef QT_DEBUG
	QLoggingCategory::defaultCategory()->setEnabled(QtDebugMsg, true);
	qDebug("Init");
#endif

	setupUi(this);
	connect(actionAbout, &QAction::triggered, this, &AndroTouch::about);
	connect(actionAbout_Qt, &QAction::triggered, qApp, &QApplication::aboutQt);
	connect(&grabber, &Grabber::grabbed, this, &AndroTouch::sshot);
	connect(&grabber, &Grabber::error, statusbar, [this](const QString &msg) { statusbar->showMessage(msg); });
	connect(actionCR, &QAction::toggled, this, [this](bool checked) { grabber.crlf = checked; });
	connect(phoneScreen, &ClickableLabel::clicked, this, &AndroTouch::touch);
	connect(phoneScreen, &ClickableLabel::unclicked, this, &AndroTouch::touch);

	grabber.start();
}

void AndroTouch::about()
{
	QMessageBox::about(this, "About AndroTouch", "AndroTouch - a Qt AndroTouch application<br>by Matteo Croce <a href=\"https://matteocroce.it/\">https://matteocroce.it/</a>");
}

Grabber::~Grabber()
{
	requestInterruption();
	wait();
}

/* runs in the grabber thread: never touch the GUI from here, use signals */
void Grabber::fail(const QString &msg)
{
	emit error(msg);
	/* don't spin when adb or the device are not available */
	msleep(1000);
}

void Grabber::run()
{
	while(!isInterruptionRequested()) {
		QProcess adb;
		adb.start("adb", {"shell", "screencap", "-p"});
		if(!adb.waitForStarted()) {
			fail("can't run adb: " + adb.errorString());
			continue;
		}
		while(!adb.waitForFinished(100))
			if(isInterruptionRequested()) {
				adb.kill();
				adb.waitForFinished();
				return;
			}
		QByteArray bytes = adb.readAllStandardOutput();

		if (crlf)
			bytes.replace("\r\n", "\n");

		if(!bytes.startsWith("\x89PNG\r\n\x1a\n")) {
			fail("invalid image, missing PNG");
			continue;
		}
		if(!bytes.endsWith("IEND\xae\x42\x60\x82")) {
			fail("invalid image, missing IEND");
			continue;
		}
#ifdef QT_DEBUG
		qDebug() << "size" << bytes.size();
#endif
		emit grabbed(bytes);
	}
}

void AndroTouch::sshot(const QByteArray &bytes)
{
	QPixmap png;
	png.loadFromData(bytes, "PNG");
	swidth = png.width();
	sheight = png.height();
	png = png.scaledToHeight(phoneScreen->height(), Qt::SmoothTransformation);
	phoneScreen->setPixmap(png);
}

void AndroTouch::touch(QMouseEvent *evt)
{
	/* no screenshot yet, the coordinates can't be scaled */
	if(phoneScreen->pixmap().isNull())
		return;

	QPoint pos = evt->position().toPoint();
	int x = pos.x() * swidth / phoneScreen->pixmap().width();
	int y = pos.y() * sheight / phoneScreen->pixmap().height();
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
			QString::number(x) << QString::number(y);
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
