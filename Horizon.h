#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Horizon.h"
#include "HTextEditor.h"

class Horizon : public QMainWindow
{
	Q_OBJECT

public:
	Horizon(QMainWindow *parent = Q_NULLPTR);

private:
	Ui::HorizonClass ui;
	QStringList file_names;
	QString file_paths;
	HTextEditor* input;

private slots:
	void file_open();
	void filelistview_clicked(QModelIndex& index);
};
