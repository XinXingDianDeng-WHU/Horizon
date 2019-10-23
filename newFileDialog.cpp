#include "stdafx.h"
#include "newFileDialog.h"

newFileDialog::newFileDialog(QWidget* parent)
{
	ui.setupUi(this);
	setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
	setFixedSize(this->width(), this->height());

	connect(ui.choosePath, SIGNAL(released()), this, SLOT(choose_Path()));
	connect(ui.Cancel, SIGNAL(released()), this, SLOT(closeDialog()));
	connect(ui.Confirm, SIGNAL(released()), this, SLOT(finish()));//ʹ��pressed()�źŻᴥ�����Σ�����취����Ϊreleased�ź�
}
newFileDialog::~newFileDialog(){}

/*���ļ���ѡ����*/
void newFileDialog::choose_Path()
{
	QString path = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("ѡ���ļ���"), ".");
	ui.Path->setText(path);
}
/*�رնԻ���*/
void newFileDialog::closeDialog()
{
	this->close();
}
/*���ѡ��*/
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