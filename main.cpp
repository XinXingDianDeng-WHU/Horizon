#include "stdafx.h"
#include "Horizon.h"
#include "ScaleWidget.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	/*ScaleWidget s;
	s.show();*/
	Horizon w;
	w.show();
	return a.exec();
}
