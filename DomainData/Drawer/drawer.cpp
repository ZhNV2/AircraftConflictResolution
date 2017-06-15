#include "drawer.h"
#include "../ConflictManager/conflict_manager.h"
#include <cassert>


namespace {
	int bit(int mask, int i) {
		return ((mask >> i) & 1);
	}
}

void Drawer::Update(double time) {
	runWidget();
}

Drawer::Drawer(std::vector<Aircraft_ptr>& aircrafts1) : aircrafts_(aircrafts1) {
	int mask = 0;
	for (auto aircraft : aircrafts1) {
		air_colors_[aircraft->GetID()] = QColor(bit(mask, 0) * 255, bit(mask, 1) * 255, bit(mask, 2) * 255);
		mask += 1;
	}
	runWidget();
}

void Drawer::runWidget() { 
	std::vector<std::pair<Segment, QColor>> segments;
	std::vector<std::pair<Point, QColor>> positions;
	for (auto aircraft : aircrafts_) {
		Track track = aircraft->GetTrack();
		for (int i = 0; i < track.GetLen() - 1; i++) {
			segments.push_back({ Segment(track.GetTrackPoint(i), track.GetTrackPoint(i + 1)), air_colors_[aircraft->GetID()] });
		}
		positions.push_back({ aircraft->GetPosition(), air_colors_[aircraft->GetID()] });

	}
	if (widget_) {
		widget_->reBuild(segments, positions);
		widget_->repaint();
	}
	else {
		widget_ = std::make_shared<Widget>(segments, positions);
		widget_->show();
	}
}



Widget::Widget(std::vector<std::pair<Segment, QColor>> segs, std::vector<std::pair<Point, QColor>> poses, QWidget * parent) : segments_(segs), positions_(poses), QWidget(parent) {}

void Widget::reBuild(std::vector<std::pair<Segment, QColor>> s, std::vector<std::pair<Point, QColor>> p) {
	segments_ = s;
	positions_ = p;
}

namespace {
	void scalePoint(Point &p, int h, int w) {
		p.x *= h / 20.;
		p.y *= w / 20.;
	}
}

void Widget::paintEvent(QPaintEvent *event) {
	QRect rec = QApplication::desktop()->screenGeometry();
	int height = rec.height();
	int width = rec.width();
	width = height;
	QPainter painter(this);
	//painter.setPen(QPen(Qt::black, 1));
	for (auto& seg : segments_) {
		painter.setPen(QPen(seg.second, 1));
		scalePoint(seg.first.first_point, width, height);
		scalePoint(seg.first.second_point, width, height);
		//() << "------------------- draw ---------------- \n";
		//qInfo() << seg.first_point.x << " " << seg.first_point.y << " " << seg.second_point.x << " " << seg.second_point.y << "\n";
		painter.drawLine(seg.first.first_point.x, seg.first.first_point.y, seg.first.second_point.x, seg.first.second_point.y);
	}
	for (auto& pt : positions_) {
		painter.setPen(QPen(pt.second, 1));
		scalePoint(pt.first, width, height);
		painter.drawEllipse(QPointF(pt.first.x, pt.first.y), 10, 10);
		painter.setPen(QPen(Qt::black, 1));
		painter.drawEllipse(QPointF(pt.first.x, pt.first.y), ConflictManager::GetMinDist() * height / 2 / 20, ConflictManager::GetMinDist() * height / 2 / 20);

	}
}