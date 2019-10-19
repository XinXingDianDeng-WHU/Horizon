#include "stdafx.h"
#include "Horizon.h"
#include "HTextEditor.h"
#include "newFileDialog.h"

Horizon::Horizon(QMainWindow* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.outputView = new HTextEditor();
	ui.verticalLayout->addWidget(ui.outputView);
	ui.tabWidget->removeTab(1);
	ui.tabWidget->removeTab(0);

	editors = new QList<HTextEditor*>();

	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(file_open()));
	connect(ui.fileListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(item_changed()));
	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(save()));
	connect(ui.actionSave_As, SIGNAL(triggered()), this, SLOT(save_as()));
	connect(ui.actionSave_All, SIGNAL(triggered()), this, SLOT(saveAll()));
	connect(ui.actionNew, SIGNAL(triggered()), this, SLOT(newFile()));
}

/*
读文件
*/
void Horizon::file_open()
{
	HTextEditor* input = new HTextEditor();
	ui.inputView = input;
	editors->append(input);
	QString fileName = QFileDialog::getOpenFileName(this,
		QString::fromLocal8Bit("打开文件"), ".", tr("Txt or CMM (*.txt *.cmm)"));
	if (fileName != NULL) {
		file_paths += fileName;
		QFileInfo fi = QFileInfo(fileName);
		file_names += fi.fileName();
		QListWidgetItem* item = new QListWidgetItem();
		item->setText(fi.fileName());
		ui.fileListWidget->addItem(item);
		ui.fileListWidget->setCurrentItem(item);

		//将选择的文件内容读取并呈现
		QFile file(fileName);
		if (!file.open(QIODevice::ReadOnly))
			return;
		QTextStream in(&file);
		while (!in.atEnd())
		{
			QString line = in.readLine();
			input->appendPlainText(line);
		}
		file.close();

		ui.tabWidget->addTab(ui.inputView, fi.fileName());
		ui.tabWidget->setCurrentIndex(ui.fileListWidget->currentRow());
	}

}

void Horizon::item_changed()
{
	ui.tabWidget->setCurrentIndex(ui.fileListWidget->currentRow());
}

/*
保存当前文件
*/
void Horizon::save()
{
	QString filename = file_paths.at(ui.tabWidget->currentIndex());
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	QTextStream out(&file);
	int index = ui.tabWidget->currentIndex();
	out << editors->at(index)->toPlainText();
	file.close();
	
}

/*
保存当前内容至指定文件
*/
void Horizon::save_as()
{
	QString fileName = QFileDialog::getSaveFileName(this,
		QString::fromLocal8Bit("选择文件"), ".", tr("Txt (*.txt)"));
	QFileInfo fi = QFileInfo(fileName);
	if (fileName != NULL) {
		QFile file(fi.fileName());
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
			return;
		QTextStream out(&file);
		int index = ui.tabWidget->currentIndex();
		out << editors->at(index)->toPlainText();
		file.close();
	}
}

/*
保存所有已打开文件
*/
void Horizon::saveAll()
{
	for (int i = 0; i < file_paths.count(); i++ ) {
		QFile file(file_paths.at(i));
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
			return;
		QTextStream out(&file);		
		out << editors->at(i)->toPlainText();
		file.close();
	}
}

void Horizon::newFile()
{
	newFileDialog* dialog = new newFileDialog(this);
	dialog->exec();
	QString fileName = dialog->new_file;


	HTextEditor* input = new HTextEditor();
	ui.inputView = input;
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
		ui.fileListWidget->addItem(item);
		ui.fileListWidget->setCurrentItem(item);

		//将选择的文件内容读取并呈现
		
		QTextStream in(&file);
		while (!in.atEnd())
		{
			QString line = in.readLine();
			input->appendPlainText(line);
		}
		file.close();

		ui.tabWidget->addTab(ui.inputView, fi.fileName());
		ui.tabWidget->setCurrentIndex(ui.fileListWidget->currentRow());
	}
}