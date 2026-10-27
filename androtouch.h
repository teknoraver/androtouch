// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2015 Matteo Croce

#ifndef ANDROTOUCH_H
#define ANDROTOUCH_H

#include <QMainWindow>
#include <QThread>
#include <QLabel>
#include <QTimer>

#include <atomic>

class ClickableLabel : public QLabel
{
Q_OBJECT
public:
	explicit ClickableLabel(QWidget *parent) : QLabel(parent) {}

signals:
	void clicked(QMouseEvent*);
	void unclicked(QMouseEvent*);
	void wheeled(QWheelEvent*);

protected:
	void mousePressEvent(QMouseEvent *event) override
	{
		emit clicked(event);
	}
	void mouseReleaseEvent(QMouseEvent *event) override
	{
		emit unclicked(event);
	}
	void wheelEvent(QWheelEvent *event) override
	{
		emit wheeled(event);
	}
};

#include "ui_androtouchwidget.h"

class Grabber : public QThread
{
	Q_OBJECT
	void run() override;
	void fail(const QString &msg);
public:
	~Grabber() override;
	std::atomic<bool> crlf = false;
signals:
	void grabbed(const QByteArray&);
	void error(const QString&);
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
	void sshot(const QByteArray&);
	void touch(QMouseEvent *evt);
	void wheel(QWheelEvent *evt);
	void scroll();
private:
	Grabber grabber;
	int lastx, lasty;
	int swidth, sheight;
	QTimer scrollTimer;
	QPoint scrollPos, scrollDelta;
};

#endif
