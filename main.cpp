#include <QApplication>
#include <QMainWindow>
#include <QPainter>
#include <QWidget>
#include <QRect>
#include <QDesktopWidget>
#include <iostream>
#include "DomainData/Aircraft/aircraft.h"
#include "Main/Runner/runner.h"


#include <qstring.h>
#include <qfile.h>
#include <qtextstream.h>
#include <cassert>

#include <ctime>

int main(int argc, char *argv[])
{

	srand(time(0));

	QApplication a(argc, argv);
	Runner runner = Runner(4);
	runner.Run();

	while (1);
	return a.exec();
}