#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Horizon.h"
#include "HTextEditor.h"

class Horizon : public QMainWindow
{
	Q_OBJECT

public:
	Horizon(QMainWindow* parent = Q_NULLPTR);

private:
	Ui::HorizonClass ui;
	QStringList file_names;
	QStringList file_paths;
	QList<HTextEditor*>* editors;

private slots:
	void file_open();
	void item_changed();
	void save();
	void save_as();
	void saveAll();
	void newFile();
};
