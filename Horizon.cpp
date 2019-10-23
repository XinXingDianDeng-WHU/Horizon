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
	verticalSplitter->setStretchFactor(0, 1);//设置inputView较大
	horizontalSplitter->setStretchFactor(1, 1);//设置verticalSplitter较大

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

//获取当前输入框中的内容
//QString Horizon::getCurContent() {
//	QString filename = file_paths.at(tabWidget->currentIndex());
//	QFile file(filename);
//	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
//	QTextStream out(&file);
//	int index = tabWidget->currentIndex();
//	file.close();
//	return editors->at(index)->toPlainText();
//}

/*读文件*/
void Horizon::fOpen()
{
	QString filePath = QFileDialog::getOpenFileName(this,
		QString::fromLocal8Bit("打开文件"), ".//res//code", tr("Txt or CMM (*.txt *.cmm)"));
	if (filePath != NULL && !file_paths.contains(filePath, Qt::CaseSensitive)) {
		file_paths += filePath;
		QFileInfo fi = QFileInfo(filePath);
		file_names += fi.fileName();
		QListWidgetItem* item = new QListWidgetItem();
		item->setText(fi.fileName());
		listWidget->addItem(item);
		listWidget->setCurrentItem(item);

		//将选择的文件内容读取并呈现
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
			+ QString::fromLocal8Bit("打开"), 3000);
	}
	else if (file_paths.contains(filePath, Qt::CaseSensitive)) {
		int index = file_paths.indexOf(filePath);
		inputView = editors->at(index);
		listWidget->setCurrentRow(index);
		tabWidget->setCurrentIndex(index);
		ui.statusBar->showMessage(QString::fromLocal8Bit("已在列表"), 3000);
	}


}

void Horizon::item_changed()
{
	tabWidget->setCurrentIndex(listWidget->currentRow());
}

/*
保存当前文件
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
保存当前内容至指定文件
*/
void Horizon::fSave_As()
{
	QString fileName = QFileDialog::getSaveFileName(this,
		QString::fromLocal8Bit("选择文件"), ".", tr("Txt (*.txt)"));
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
保存所有已打开文件
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

		//将选择的文件内容读取并呈现

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

/*关闭文件*/
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



/*setStrechFactor，第一个参数指内部空间索引，第二个参数为相对比例
例如上文horizontalSplitter中，listWidget索引为0，verticalSplitter索引为1
默认情况下，所有控件strench比例属性都是0
例如初始时verticalSplitter中inputView：outputView=0：0，即均分
如果1：0，则前者大后者小，比例为默认比例
如果比例均不为0，例如3：2，则大小比就是3：2*/
