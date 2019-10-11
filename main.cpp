#include "stdafx.h"
#include "Horizon.h"
#include "HTextEditor.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Horizon w;
	w.show();
	return a.exec();
}
