#include "drawer.h"
#include <cassert>


void Drawer::Update(double time) {
	widget_->close();
	runWidget();
}

Drawer::Drawer(std::vector<Aircraft_ptr>& aircrafts1) : aircrafts_(aircrafts1) {
	runWidget();
}

void Drawer::runWidget() { 
	std::vector<Segment> segments;
	std::vector<Point> positions;
	for (auto aircraft : aircrafts_) {
		Track track = aircraft->GetTrack();
		for (int i = 0; i < track.GetLen() - 1; i++) {
			segments.push_back(Segment(track.GetTrackPoint(i), track.GetTrackPoint(i + 1)));
		}
		positions.push_back(aircraft->GetPosition());
	}
	widget_ = std::make_shared<Widget>(segments, positions);
	widget_->show();
}



Widget::Widget(std::vector<Segment> segs, std::vector<Point> poses, QWidget * parent) : segments_(segs), positions_(poses), QWidget(parent) {}

namespace {
	void scalePoint(Point &p, int h, int w) {
		p.x *= h / 10;
		p.y *= w / 10;
	}
}

void Widget::paintEvent(QPaintEvent *event) {
	QRect rec = QApplication::desktop()->screenGeometry();
	int height = rec.height();
	int width = rec.width();
	width = height;
	QPainter painter(this);
	painter.setPen(QPen(Qt::black, 2));
	for (auto& seg : segments_) {
		scalePoint(seg.first_point, width, height);
		scalePoint(seg.second_point, width, height);
		qInfo() << "------------------- draw ---------------- \n";
		qInfo() << seg.first_point.x << " " << seg.first_point.y << " " << seg.second_point.x << " " << seg.second_point.y << "\n";
		painter.drawLine(seg.first_point.x, seg.first_point.y, seg.second_point.x, seg.second_point.y);
	}
	for (auto& pt : positions_) {
		scalePoint(pt, width, height);
		painter.drawEllipse(QPointF(pt.x, pt.y), 10, 10);
	}
}