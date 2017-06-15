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
	Widget(std::vector<std::pair<Segment, QColor>> segments, std::vector<std::pair<Point, QColor>> positions, QWidget *parent = 0);
	void reBuild(std::vector<std::pair<Segment, QColor>>, std::vector<std::pair<Point, QColor>>);
protected:
	void paintEvent(QPaintEvent *event);
private:
	std::vector<std::pair<Segment, QColor> > segments_;
	std::vector<std::pair<Point, QColor> > positions_;
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
	std::map<int, QColor> air_colors_;
};


typedef std::shared_ptr<Drawer> Drawer_ptr;
