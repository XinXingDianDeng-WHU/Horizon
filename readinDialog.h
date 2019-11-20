#pragma once
#include <qpushbutton.h>
#include "ui_readinDialog.h"
namespace Ui {
	class readinDialog;
}

class readinDialog : public QDialog
{
	Q_OBJECT;
public:
	readinDialog(QWidget* parent = 0);
	~readinDialog();
	QString inVar;
public slots:
	bool inVarChecked();
	QString getCommand();
private:
	Ui::readinDialog rdinDialog;
private slots:
	void clear();
};

