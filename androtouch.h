#ifndef ANDROTOUCH_H
#define ANDROTOUCH_H

#include <QMainWindow>
#include <QProcess>
#include <QThread>
#include <QLabel>

class ClickableLabel : public QLabel
{ 
Q_OBJECT
public:
	explicit ClickableLabel(QWidget *parent = 0) {};

signals:
	void clicked(QMouseEvent*);

protected:
	void mousePressEvent(QMouseEvent *event)
	{
		emit clicked(event);
	}
};

#include "ui_androtouchwidget.h"

class Grabber : public QThread
{
	Q_OBJECT
	QProcess adb;
	void run();
	QByteArray png;
public:
	Grabber();
signals:
	void grabbed(QByteArray*);
};

class AndroTouch: public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT
public:
	AndroTouch();
private slots:
	void about();
	void sshot(QByteArray*);
	void touch(QMouseEvent *evt);
private:
	Grabber grabber;
};

#endif
