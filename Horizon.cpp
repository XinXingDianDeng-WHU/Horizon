#include "stdafx.h"
#include "Horizon.h"
#include "HTextEditor.h"

Horizon::Horizon(QMainWindow* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//ui.inputView = input;
	ui.outputView = new HTextEditor();
	ui.verticalLayout->addWidget(ui.outputView);
	ui.tabWidget->removeTab(1);
	ui.tabWidget->removeTab(0);
	//ui.tabWidget->addTab(ui.inputView,"tab3");

	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(file_open()));
	//connect(ui.fileListView, SIGNAL(clicked(QModelIndex)), this, SLOT(filelistview_clicked(QModelIndex & index)));
	//connect(ui.fileListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(filelist_clicked(QListWidgetItem*)));
	connect(ui.fileListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(item_changed()));
}

/*
读文件
*/
void Horizon::file_open()
{
	HTextEditor* input = new HTextEditor();
	ui.inputView = input;
	QString fileName = QFileDialog::getOpenFileName(this,
		QString::fromLocal8Bit("打开文件"), ".", tr("Txt or CMM (*.txt *.cmm)"));
	if (fileName != NULL) {
		//后续功能：检测文件是否已经打开
		file_paths += fileName;
		QFileInfo fi = QFileInfo(fileName);
		file_names += fi.fileName();
		/*QStringListModel* model = new QStringListModel(file_names);
		ui.fileListView->setModel(model);
		QModelIndex qindex = model->index(file_names.count() - 1, 0);
		ui.fileListView->setCurrentIndex(qindex);*/
		QListWidgetItem* item = new QListWidgetItem();
		item->setText(fi.fileName());
		ui.fileListWidget->addItem(item);
		ui.fileListWidget->setCurrentItem(item);

		//清空编辑区代码
		//input->clear();
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
	}

}

void Horizon::filelist_clicked(QListWidgetItem* item)
{
	qDebug() << "000000";
	if (item != ui.fileListWidget->currentItem()) {
		qDebug() << "11111111";
		//input->clear();
		QString currentFile = file_paths.at(ui.fileListWidget->currentRow());
		QFile file(currentFile);
		if (!file.open(QIODevice::ReadOnly))
			return;
		QTextStream in(&file);
		while (!in.atEnd())
		{
			QString line = in.readLine();
			//input->appendPlainText(line);
		}
		file.close();
	}
}

void Horizon::item_changed()
{
	//input->clear();
	QString currentFile = file_paths.at(ui.fileListWidget->currentRow());
	QFile file(currentFile);
	if (!file.open(QIODevice::ReadOnly))
		return;
	QTextStream in(&file);
	while (!in.atEnd())
	{
		QString line = in.readLine();
		//input->appendPlainText(line);
	}
	file.close();
}