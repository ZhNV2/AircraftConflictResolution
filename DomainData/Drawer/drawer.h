#pragma once
#include "../domain_object.h"
#include "../Aircraft/aircraft.h"
#include "../Geometry/Segment/segment.h"
#include <vector>
#include <QApplication>
#include <QMainWindow>
#include <qdebug.h>
#include <QPainter>
#include <QWidget>
#include <QRect>
#include <QDesktopWidget>
#include <iostream>

class Widget : public QWidget
{
public:
	Widget(std::vector<Segment> segments, std::vector<Point> positions, QWidget *parent = 0);
protected:
	void paintEvent(QPaintEvent *event);
private:
	std::vector<Segment> segments_;
	std::vector<Point> positions_;
};

typedef std::shared_ptr<Widget> Widget_ptr;

class Drawer : public DomainObject {
public:
	void Update(double time);
	Drawer(std::vector<Aircraft_ptr>&);
private:
	std::vector<Aircraft_ptr> aircrafts_;
	Widget_ptr widget_;
	void runWidget();
};


typedef std::shared_ptr<Drawer> Drawer_ptr;
