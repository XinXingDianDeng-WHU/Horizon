#include "stdafx.h"
#include "Horizon.h"
#include "ScaleWidget.h"
#include <QtWidgets/QApplication>
#include "Parser/MyParser.h"

int main(int argc, char *argv[]) {
	QApplication horizonApp(argc, argv);
	Horizon horizon;
	horizon.show(); 
	return horizonApp.exec();
}
