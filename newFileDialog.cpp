#include "stdafx.h"
#include "newFileDialog.h"

newFileDialog::newFileDialog(QWidget* parent)
{
	nfDialog.setupUi(this);
	setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
	setFixedSize(this->width(), this->height());

	connect(nfDialog.choosePath, SIGNAL(released()), this, SLOT(choose_Path()));
	connect(nfDialog.Cancel, SIGNAL(released()), this, SLOT(closeDialog()));
	connect(nfDialog.Confirm, SIGNAL(released()), this, SLOT(finish()));//ʹ��pressed()�źŻᴥ�����Σ�����취����Ϊreleased�ź�
}
newFileDialog::~newFileDialog(){}

/*���ļ���ѡ����*/
void newFileDialog::choose_Path()
{
	QString path = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("ѡ���ļ���"), ".");
	nfDialog.Path->setText(path);
}
/*�رնԻ���*/
void newFileDialog::closeDialog()
{
	this->close();
}
/*���ѡ��*/
void newFileDialog::finish()
{
	if (nfDialog.FileName->text() != NULL && nfDialog.Path->text() != NULL) {
		new_file = nfDialog.Path->text() + "/" + nfDialog.FileName->text() + ".txt";
		QFile file(new_file);
		if (!file.exists()) {
			this->close();
		} else {
			nfDialog.message_Lable->setText(QString::fromLocal8Bit("�ļ��Ѵ��ڣ�"));
		}
	}
}