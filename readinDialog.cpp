#include "readinDialog.h"
#include <qexception.h>

readinDialog::readinDialog(QWidget*parent)
	:inVar("") {
	rdinDialog.setupUi(this);
	setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
	setFixedSize(this->width(), this->height());

	connect(rdinDialog.Confirm, SIGNAL(released()), this, SLOT(inVarChecked()));
	//connect(rdinDialog.Confirm, SIGNAL(released()), this, SLOT(getCommand()));
	connect(rdinDialog.Cancel, SIGNAL(released()), this, SLOT(clear()));
}
readinDialog::~readinDialog() {}

bool readinDialog::inVarChecked() {
	QString invar= rdinDialog.Readin->text();
	if (invar != NULL) {
		QRegExp isDouble("^-?[0-9]*.[0-9]+$"), isInt("^-?[0-9]+$");
		if ( isInt.exactMatch(invar) || isDouble.exactMatch(invar)) {
			rdinDialog.message_Label->setText("");
			inVar = invar;
			this->close();
			return true;
		} else {
			rdinDialog.message_Label->setText(QString::fromLocal8Bit("值必须为数字！"));
		}
	} else {
		rdinDialog.message_Label->setText(QString::fromLocal8Bit("值不可空！"));
	}
	return false;
}

QString readinDialog::getCommand() {
	this->close();
	return rdinDialog.Readin->text();
}
void readinDialog::clear() {
	if (rdinDialog.Readin->text() != NULL)rdinDialog.Readin->setText("");
}