#ifndef ANDROTOUCH_H
#define ANDROTOUCH_H

#include <QMainWindow>
#include <QProcess>
#include <QThread>

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
private:
	Grabber grabber;
};

#endif
