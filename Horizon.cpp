#include "stdafx.h"
#include "Horizon.h"

Horizon::Horizon(QMainWindow* parent)
	: QMainWindow(parent)
	, editors(new QList<SmartEdit*>())
	, horizontalSplitter(new QSplitter(Qt::Horizontal, this))
	, verticalSplitter(new QSplitter(Qt::Vertical, horizontalSplitter))
	, listWidget(new QListWidget(horizontalSplitter))
	, tabWidget(new QTabWidget(verticalSplitter))
	, inputView(NULL)
	, outputView(new QTextEdit(verticalSplitter))  {

	ui.setupUi(this); 
	horizontalSplitter->addWidget(listWidget); horizontalSplitter->addWidget(verticalSplitter);
	verticalSplitter->addWidget(tabWidget); verticalSplitter->addWidget(outputView);
	verticalSplitter->setStretchFactor(0, 1);//����inputView�ϴ�
	horizontalSplitter->setStretchFactor(1, 1);//����verticalSplitter�ϴ�

	connect(listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(item_changed()));
	connect(ui.actionNew, SIGNAL(triggered()), this, SLOT(fNew()));
	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(fOpen()));
	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(fSave()));
	connect(ui.actionSave_As, SIGNAL(triggered()), this, SLOT(fSave_As()));
	connect(ui.actionSave_All, SIGNAL(triggered()), this, SLOT(fSave_All()));
	connect(ui.actionClose, SIGNAL(triggered()), this, SLOT(fClose()));
	connect(ui.actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

	this->setCentralWidget(horizontalSplitter);
	this->setMinimumSize(1200, 600); 
}
Horizon::~Horizon() {}

//��ȡ��ǰ������е�����
//QString Horizon::getCurContent() {
//	QString filename = file_paths.at(tabWidget->currentIndex());
//	QFile file(filename);
//	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
//	QTextStream out(&file);
//	int index = tabWidget->currentIndex();
//	file.close();
//	return editors->at(index)->toPlainText();
//}

/*���ļ�*/
void Horizon::fOpen()
{
	QString filePath = QFileDialog::getOpenFileName(this,
		QString::fromLocal8Bit("���ļ�"), ".//res//code", tr("Txt or CMM (*.txt *.cmm)"));
	if (filePath != NULL && !file_paths.contains(filePath, Qt::CaseSensitive)) {
		file_paths += filePath;
		QFileInfo fi = QFileInfo(filePath);
		file_names += fi.fileName();
		QListWidgetItem* item = new QListWidgetItem();
		item->setText(fi.fileName());
		listWidget->addItem(item);
		listWidget->setCurrentItem(item);

		//��ѡ����ļ����ݶ�ȡ������
		QFile file(filePath);
		if (!file.open(QIODevice::ReadOnly)) return;
		QTextStream in(&file);
		QString all = in.readAll();
		inputView = new SmartEdit();
		inputView->appendPlainText(all);
		file.close();
		tabWidget->addTab(inputView, fi.fileName());
		tabWidget->setCurrentIndex(listWidget->currentRow());
		editors->append(inputView);
		ui.statusBar->showMessage(fi.fileName()
			+ QString::fromLocal8Bit("��"), 3000);
	}
	else if (file_paths.contains(filePath, Qt::CaseSensitive)) {
		int index = file_paths.indexOf(filePath);
		inputView = editors->at(index);
		listWidget->setCurrentRow(index);
		tabWidget->setCurrentIndex(index);
		ui.statusBar->showMessage(QString::fromLocal8Bit("�����б�"), 3000);
	}


}

void Horizon::item_changed()
{
	tabWidget->setCurrentIndex(listWidget->currentRow());
}

/*
���浱ǰ�ļ�
*/
void Horizon::fSave()
{
	QString filename = file_paths.at(tabWidget->currentIndex());
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	QTextStream out(&file);
	int index = tabWidget->currentIndex();
	out << editors->at(index)->toPlainText();
	file.close();
}

/*
���浱ǰ������ָ���ļ�
*/
void Horizon::fSave_As()
{
	QString fileName = QFileDialog::getSaveFileName(this,
		QString::fromLocal8Bit("ѡ���ļ�"), ".", tr("Txt (*.txt)"));
	QFileInfo fi = QFileInfo(fileName);
	if (fileName != NULL) {
		QFile file(fi.fileName());
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
			return;
		QTextStream out(&file);
		int index = tabWidget->currentIndex();
		out << editors->at(index)->toPlainText();
		file.close();
	}
}

/*
���������Ѵ��ļ�
*/
void Horizon::fSave_All()
{
	for (int i = 0; i < file_paths.count(); i++) {
		QFile file(file_paths.at(i));
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
			return;
		QTextStream out(&file);
		out << editors->at(i)->toPlainText();
		file.close();
	}
}

void Horizon::fNew()
{
	newFileDialog* dialog = new newFileDialog(this);
	dialog->exec();
	QString fileName = dialog->new_file; 
	SmartEdit* input = new SmartEdit();
	inputView = input;
	editors->append(input);
	if (fileName != NULL) {
		QFile file(fileName);
		if (!file.open(QIODevice::NewOnly | QIODevice::ReadWrite))
			return;
		file_paths += fileName;
		QFileInfo fi = QFileInfo(fileName);
		file_names += fi.fileName();
		QListWidgetItem* item = new QListWidgetItem();
		item->setText(fi.fileName());
		listWidget->addItem(item);
		listWidget->setCurrentItem(item);

		//��ѡ����ļ����ݶ�ȡ������

		QTextStream in(&file);
		while (!in.atEnd())
		{
			QString line = in.readLine();
			input->appendPlainText(line);
		}
		file.close();

		tabWidget->addTab(inputView, fi.fileName());
		tabWidget->setCurrentIndex(listWidget->currentRow());
	}
}

/*�ر��ļ�*/
void Horizon::fClose()
{
	if (tabWidget->count() > 0)
	{
		int index = tabWidget->currentIndex();
		tabWidget->removeTab(index);
		file_names.removeAt(index);
		file_paths.removeAt(index);
		editors->removeAt(index);
		listWidget->takeItem(index);
	}
}



/*setStrechFactor����һ������ָ�ڲ��ռ��������ڶ�������Ϊ��Ա���
��������horizontalSplitter�У�listWidget����Ϊ0��verticalSplitter����Ϊ1
Ĭ������£����пؼ�strench�������Զ���0
�����ʼʱverticalSplitter��inputView��outputView=0��0��������
���1��0����ǰ�ߴ����С������ΪĬ�ϱ���
�����������Ϊ0������3��2�����С�Ⱦ���3��2*/
