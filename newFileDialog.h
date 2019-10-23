#pragma once
#include <qpushbutton.h>
#include "ui_newFileDialog.h"
namespace Ui {
	class newFileDialog;
}
class newFileDialog :
	public QDialog
{
	Q_OBJECT;
public :
	newFileDialog(QWidget* parent = 0);
	~newFileDialog();
	QString new_file;
private:
	Ui::newFileDialog ui;
private slots:
	void choose_Path();
	void closeDialog();
	void finish();
};

