#include "stdafx.h"
#include "newFileDialog.h"

newFileDialog::newFileDialog(QWidget* parent)
{
	ui.setupUi(this);
	setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
	setFixedSize(this->width(), this->height());

	connect(ui.choosePath, SIGNAL(released()), this, SLOT(choose_Path()));
	connect(ui.Cancel, SIGNAL(released()), this, SLOT(closeDialog()));
	connect(ui.Confirm, SIGNAL(released()), this, SLOT(finish()));//使用pressed()信号会触发两次，解决办法：改为released信号
}
newFileDialog::~newFileDialog(){}

/*打开文件夹选择器*/
void newFileDialog::choose_Path()
{
	QString path = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("选择文件夹"), ".");
	ui.Path->setText(path);
}
/*关闭对话框*/
void newFileDialog::closeDialog()
{
	this->close();
}
/*完成选择*/
void newFileDialog::finish()
{
	if (ui.FileName->text() != NULL && ui.Path->text() != NULL) {
		new_file = ui.Path->text() + "/" + ui.FileName->text() + ".txt";
		QFile file(new_file);
		if (!file.exists()) {
			this->close();
		}
	}
}