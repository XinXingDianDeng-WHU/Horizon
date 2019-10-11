#include "stdafx.h"
#include "Horizon.h"
#include "HTextEditor.h"

Horizon::Horizon(QMainWindow *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	input = new HTextEditor();
	ui.inputView = input;
	ui.outputView = new HTextEditor();
	ui.verticalLayout->addWidget(ui.inputView);
	ui.verticalLayout->addWidget(ui.outputView);

	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(file_open()));
	connect(ui.fileListView, SIGNAL(clicked(QModelIndex)), this, SLOT(filelistview_clicked(QModelIndex & index)));
}

/*
读文件
*/
void Horizon::file_open()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		QString::fromLocal8Bit("打开文件"), ".", tr("Txt or CMM (*.txt *.cmm)"));
	if (fileName != NULL) {
		//后续功能：检测文件是否已经打开
		file_paths += fileName;
		QFileInfo fi = QFileInfo(fileName);
		file_names += fi.fileName();
		QStringListModel* model = new QStringListModel(file_names);
		ui.fileListView->setModel(model);
		QModelIndex qindex = model->index(file_names.count() - 1, 0);
		ui.fileListView->setCurrentIndex(qindex);

		//清空编辑区代码
		input->clear();
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
	}
	
}

void Horizon::filelistview_clicked(QModelIndex& index)
{
	qDebug() << "000000";
	if (index != ui.fileListView->currentIndex()) {
		qDebug() << "11111111";
		input->clear();
		QString currentFile = file_paths.at(index.row());
		QFile file(currentFile);
		if (!file.open(QIODevice::ReadOnly))
			return;
		QTextStream in(&file);
		while (!in.atEnd())
		{
			QString line = in.readLine();
			input->appendPlainText(line);
		}
		file.close();
	}
}