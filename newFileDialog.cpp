#include "stdafx.h"
#include "newFileDialog.h"

newFileDialog::newFileDialog(QWidget* parent)
{
	gridLayout = new QGridLayout(this);
	hboxLayout = new QHBoxLayout();
	Path = new QLineEdit(this);
	Name = new QLineEdit(this);
	path_Label = new QLabel(this);
	name_Label = new QLabel(this);
	Finish = new QPushButton(this);
	choosePath = new QPushButton(this);
	Cancle = new QPushButton(this);

	path_Label->setText(QString::fromLocal8Bit("�洢·����"));
	path_Label->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	path_Label->setFixedSize(100, 20);

	name_Label->setText(QString::fromLocal8Bit("�ļ�����"));
	name_Label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	name_Label->setFixedSize(100, 30);

	Path->setFocusPolicy(Qt::NoFocus);
	Path->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	Path->setFixedSize(300, 30);

	Name->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	Name->setFixedSize(300, 30);

	choosePath->setFixedSize(30, 30);

	Finish->setFixedSize(80, 30);
	
	Cancle->setText(QString::fromLocal8Bit("ȡ��"));
	Cancle->setFixedSize(80, 30);

	Finish->setText(QString::fromLocal8Bit("�½�"));
	Finish->setFixedSize(80, 30);

	hboxLayout->setSpacing(100);
	hboxLayout->addWidget(Finish);
	hboxLayout->addWidget(Cancle);

	gridLayout->setSpacing(10);
	gridLayout->addWidget(path_Label, 0, 0);
	gridLayout->addWidget(Path, 0, 1);
	gridLayout->addWidget(choosePath, 0, 2);
	gridLayout->addWidget(name_Label, 1, 0);
	gridLayout->addWidget(Name, 1, 1);
	gridLayout->addLayout(hboxLayout, 2, 1);

	setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
	setFixedSize(this->width(), this->height());

	connect(choosePath, SIGNAL(released()), this, SLOT(choose_Path()));
	connect(Cancle, SIGNAL(released()), this, SLOT(closeDialog()));
	connect(Finish, SIGNAL(released()), this, SLOT(finish()));//ʹ��pressed()�źŻᴥ�����Σ�����취����Ϊreleased�ź�
}
newFileDialog::~newFileDialog(){}

/*���ļ���ѡ����*/
void newFileDialog::choose_Path()
{
	QString path = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("ѡ���ļ���"), ".");
	Path->setText(path);
}
/*�رնԻ���*/
void newFileDialog::closeDialog()
{
	this->close();
}
/*���ѡ��*/
void newFileDialog::finish()
{
	if (Name->text() != NULL && Path->text() != NULL) {
		new_file = Path->text() + "/" + Name->text() + ".txt";
		this->close();
	}
}