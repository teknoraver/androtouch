#ifndef ANDROTOUCH_H
#define ANDROTOUCH_H

#include <QMainWindow>
#include <QProcess>

#include "ui_androtouchwidget.h"

class AndroTouch: public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT
public:
	AndroTouch();
	~AndroTouch();
private slots:
	void about();
	void sshot();

private:
	QProcess adb;
};

#endif
