#ifndef ANDROTOUCH_H
#define ANDROTOUCH_H

#include <QMainWindow>
#include <QThread>
#include <QLabel>

class ClickableLabel : public QLabel
{
Q_OBJECT
public:
	explicit ClickableLabel(QWidget *parent) : QLabel(parent) {}

signals:
	void clicked(QMouseEvent*);
	void unclicked(QMouseEvent*);

protected:
	void mousePressEvent(QMouseEvent *event) override
	{
		emit clicked(event);
	}
	void mouseReleaseEvent(QMouseEvent *event) override
	{
		emit unclicked(event);
	}
};

#include "ui_androtouchwidget.h"

class AndroTouch;

class Grabber : public QThread
{
	Q_OBJECT
	void run() override;
	QByteArray png;
	Ui::MainWindow *ui;
public:
	Grabber(Ui::MainWindow *u) {
		ui = u;
	}
signals:
	void grabbed(QByteArray*);
};

class AndroTouch: public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT
public:
	AndroTouch();
protected:
	void keyPressEvent(QKeyEvent *event) override;
private slots:
	void about();
	void sshot(QByteArray*);
	void touch(QMouseEvent *evt);
private:
	Grabber grabber;
	int lastx, lasty;
	int swidth, sheight;
};

#endif
