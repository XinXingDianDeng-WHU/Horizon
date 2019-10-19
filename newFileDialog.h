#pragma once
#include "F:\Qt\5.13.1\msvc2017_64\include\QtWidgets\qdialog.h"
#include <qpushbutton.h>
class newFileDialog :
	public QDialog
{
	Q_OBJECT;
protected:
	QGridLayout* gridLayout;
	QHBoxLayout* hboxLayout;
	QPushButton* Finish;
	QPushButton* Cancle;
	QPushButton* choosePath;
	QLineEdit* Path;
	QLineEdit* Name;
	QLabel* path_Label;
	QLabel* name_Label;
public :
	newFileDialog(QWidget* parent = 0);
	~newFileDialog();
	QString new_file;
private slots:
	void choose_Path();
	void closeDialog();
	void finish();
};

