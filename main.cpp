#include <QApplication>
#include <QMainWindow>
#include <QPainter>
#include <QWidget>
#include <QRect>
#include <QDesktopWidget>
#include <iostream>
#include "DomainData/Aircraft/aircraft.h"
#include "Main/Runner/runner.h"

class Widget : public QWidget
{
protected:
	void paintEvent(QPaintEvent *event)
	{
		QRect rec = QApplication::desktop()->screenGeometry();
		int height = rec.height();
		int width = rec.width();
		QPainter painter(this);
		painter.setPen(QPen(Qt::black, 2));
		painter.drawLine(0, 0, width, height);
	}
};

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Runner runner = Runner(2);
	runner.Run();

	//Widget w;
	//w.show();

	//	w.close();
	return a.exec();
}

/*#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}*/
