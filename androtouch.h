#ifndef ANDROTOUCH_H
#define ANDROTOUCH_H

#include <QMainWindow>
#include <QThread>
#include <QLabel>

class ClickableLabel : public QLabel
{ 
Q_OBJECT
public:
	explicit ClickableLabel(QWidget *) {};

signals:
	void clicked(QMouseEvent*);
	void unclicked(QMouseEvent*);

protected:
	void mousePressEvent(QMouseEvent *event)
	{
		emit clicked(event);
	}
	void mouseReleaseEvent(QMouseEvent *event)
	{
		emit unclicked(event);
	}
};

#include "ui_androtouchwidget.h"

class Grabber : public QThread
{
	Q_OBJECT
	void run();
	QByteArray png;
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
	int lastx, lasty;
};

#endif
