#include "stdafx.h"
#include "Horizon.h"
#include "QProcess"
#include<QDesktopServices>
#include<QUrl>

Horizon::Horizon(QMainWindow* parent)
	: QMainWindow(parent)
	, editors(new QList<SmartEdit*>())
	, horizontalSplitter(new QSplitter(Qt::Horizontal, this))
	, verticalSplitter(new QSplitter(Qt::Vertical, horizontalSplitter))
	, listWidget(new QListWidget(horizontalSplitter))
	, inputTab(new QTabWidget(verticalSplitter))
	, outputTab(new QTabWidget(verticalSplitter))
	, consoleView(new QTextEdit(outputTab))
	, IEView(new QTextEdit(outputTab))
	, curDateTimeWidget(new QLabel())
	, vars(new QList<Var*>())
	, timer(new QTimer(this))
	, parser(NULL) {

	horizonUi.setupUi(this); 
	timer->start(1000);
	horizontalSplitter->addWidget(listWidget); 
	horizontalSplitter->addWidget(verticalSplitter);
	//inputTab->setTabsClosable(true);
	verticalSplitter->addWidget(inputTab); 
	verticalSplitter->addWidget(outputTab);
	outputTab->addTab(consoleView, QString::fromLocal8Bit( "静态语义分析结果"));
	outputTab->addTab(IEView,QString::fromLocal8Bit("解释执行结果"));
	consoleView->setReadOnly(true);
	consoleView->setWordWrapMode(QTextOption::NoWrap);
	IEView->setReadOnly(true);
	IEView->setWordWrapMode(QTextOption::NoWrap);
	verticalSplitter->setStretchFactor(0, 1);//<commit0>请查看res\tools\commit.txt
	horizontalSplitter->setStretchFactor(1, 1);
	this->setCentralWidget(horizontalSplitter);
	this->setMinimumSize(900, 540);
	curDateTimeWidget->setAlignment(Qt::AlignRight);
	horizonUi.statusBar->addPermanentWidget(curDateTimeWidget);
	horizonUi.statusBar->showMessage(QString::fromLocal8Bit("Horizon 感谢您的使用"), 6000);

	connect(horizonUi.actionQuit, SIGNAL(triggered()), this, SLOT(fQuit()));
	connect(horizonUi.actionNew, SIGNAL(triggered()), this, SLOT(fNew()));
	connect(horizonUi.actionOpen, SIGNAL(triggered()), this, SLOT(fOpen()));
	connect(horizonUi.actionSave, SIGNAL(triggered()), this, SLOT(fSave()));
	connect(horizonUi.actionSave_All, SIGNAL(triggered()), this, SLOT(fSave_All()));
	connect(horizonUi.actionClose, SIGNAL(triggered()), this, SLOT(fClose()));
	connect(horizonUi.actionRun, SIGNAL(triggered()), this, SLOT(fSafeRun()));
	connect(horizonUi.actionCancelDebug, SIGNAL(triggered()), this, SLOT(fCancelDebug()));
	connect(horizonUi.actionUndo, SIGNAL(triggered()), this, SLOT(fUndo()));
	connect(horizonUi.actionRedo, SIGNAL(triggered()), this, SLOT(fRedo()));
	connect(horizonUi.actionCut, SIGNAL(triggered()), this, SLOT(fCut()));
	connect(horizonUi.actionCopy, SIGNAL(triggered()), this, SLOT(fCopy()));
	connect(horizonUi.actionPaste, SIGNAL(triggered()), this, SLOT(fPaste()));
	connect(horizonUi.actionSelect_All, SIGNAL(triggered()), this, SLOT(fSelectAll()));
	connect(horizonUi.actionBreak, SIGNAL(triggered()), this, SLOT(fBreak()));
	connect(horizonUi.actionCancelBreak, SIGNAL(triggered()), this, SLOT(fCancelBreak()));
	connect(horizonUi.actionLineDebug, SIGNAL(triggered()), this, SLOT(fLineDebug()));
	connect(horizonUi.actionPointDebug, SIGNAL(triggered()), this, SLOT(fPointDebug()));
	connect(horizonUi.actionAbout, SIGNAL(triggered()), this, SLOT(fAbout()));
	connect(listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(tab_Synto_List(int)));
	connect(timer, SIGNAL(timeout()), this, SLOT(showCurDateTime()));
	connect(timer, SIGNAL(timeout()), this, SLOT(fStaticSemanticAnalysis()));
}
Horizon::~Horizon() {
	delete parser;
	delete consoleView;
	delete IEView;
	delete outputTab;
	delete inputTab;
	delete listWidget;
	delete verticalSplitter;
	delete horizontalSplitter;
	editors->clear();
	//delete editors;
}

/*获取当前时间*/
void Horizon::showCurDateTime() {
	curDateTimeWidget->setText(QDate::currentDate().toString()
		+ " / " + QTime::currentTime().toString() + " ");
}
/*tab不空且有内容*/
bool Horizon::tabNotEmpty() {
	return (inputTab->count() > 0);
}

/*菜单按钮退出*/
int Horizon::fQuit() {
	int choose = -1;
	if (tabNotEmpty()) {
		choose = QMessageBox::question(this
			, QString::fromLocal8Bit("退出")
			, QString::fromLocal8Bit("是否保存所有文件？")
			, QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		if (QMessageBox::Yes == choose)  fSave_All();
		if (choose != QMessageBox::Cancel) qApp->quit();
	} else {
		qApp->quit();
	}
	return choose;
}

/*窗口按钮退出*/
void Horizon::closeEvent(QCloseEvent* event) {//<commit3>请查看res\tools\commit.txt
	if (QMessageBox::Cancel == fQuit())event->ignore();
}
/*使用说明*/
void Horizon::fAbout() {
	QMessageBox::information(this, QString::fromLocal8Bit("使用说明"), QString::fromLocal8Bit("Horizon为您服务：\n打开文件按下F1在当前行添加断点\n按F2取消当前行断点"));
}
/*调试*/
void Horizon::fSafeRun() {
	if (fSave() > 0) {
		int index = inputTab->currentIndex();
		SmartEdit* curEdit = editors->at(index);
		try {
			parser = new MyParser(file_paths.at(index).toStdString().c_str()
				, SLR1Txt, MyProductions, "empty", true);
			parser->Parse(); 
			consoleView->append(QTime::currentTime().toString()
				+ " <" + file_names.at(index) + "> "
				+ QString::fromLocal8Bit("语法分析和静态语义分析通过"));
		} 
		catch (Exception e) {
			//跳转至错误行,报错行号本身与实际行号对应，而block号则是从0开始，因此e.row=blockNumber+1
			curEdit->setTextCursor(QTextCursor(curEdit->document()->findBlockByNumber(e.row-1)));
			consoleView->append(QTime::currentTime().toString()
				+ " <" + file_names.at(index) + "> "
				+ QString::fromLocal8Bit(e.print().c_str()));
		}
		QString standardOutput = fTest();
		vars = ReadVal(standardOutput);
		FindFirstPoint(vars, QList<int>());
		horizonUi.statusBar->showMessage(QString::fromLocal8Bit("运行完成"), 3000);
	}
}
/*取消调试*/
void Horizon::fCancelDebug() {
	int index = inputTab->currentIndex();
	SmartEdit* curEdit = editors->at(index);
	curEdit->setEnabled(true);
	IEView->clear();
	outputTab->setCurrentIndex(0);
	vars->clear();
	horizonUi.actionCancelDebug->setEnabled(false);
	horizonUi.actionRun->setEnabled(true);
}

/*inputView、Tab和list、editors同步*/
//如果涉及connect函数中没有处理好越界问题，则会引发程序崩溃
void Horizon::tab_Synto_List(int index) {
	if (index >= 0) { 
		inputTab->setCurrentIndex(index); 
	}
}

/*静态语义分析*/
void Horizon::fStaticSemanticAnalysis() {
	if (tabNotEmpty()) {
		SmartEdit* curEdit = editors->at(inputTab->currentIndex());
		//多于150行不主动执行静态语义分析
		if (curEdit->document()->lineCount() <= 150) {
			QString curText = curEdit->toPlainText() + '\0';
			try {
				parser = new MyParser(curText.toStdString().c_str()
					, SLR1Txt, MyProductions, "empty", false);
				parser->Parse();
				curEdit->exceptionRow = -1;
				horizonUi.actionBreak->setEnabled(true);
				horizonUi.actionCancelBreak->setEnabled(true);
			}
			catch (Exception e) {
				curEdit->exceptionRow = e.row;
				horizonUi.actionBreak->setEnabled(false);
				horizonUi.actionCancelBreak->setEnabled(false);
			}
		}
	}
}

//新建文件，path/name/editors/list/tab
void Horizon::fNew()
{
	newFileDialog* dialog = new newFileDialog(this);
	dialog->exec();
	QString filePath = dialog->new_file; 
	if (filePath != NULL) {
		QFile file(filePath);
		if (!file.open(QIODevice::NewOnly | QIODevice::ReadWrite)) 
			return;
		file_paths += filePath;
		QFileInfo fi = QFileInfo(filePath);
		file_names += fi.fileName();
		//将选择的文件内容读取并呈现
		QTextStream in(&file);
		in.setCodec(QTextCodec::codecForName("utf-8"));
		QString line = in.readAll();
		SmartEdit*curEdit = new SmartEdit();
		curEdit->appendPlainText(line);
		file.close();
		editors->append(curEdit);

		QListWidgetItem* item = new QListWidgetItem();
		item->setText(fi.fileName());
		listWidget->addItem(item);
		inputTab->addTab(curEdit, fi.fileName());
		//<commit4>请查看res\tools\commit.txt
		listWidget->setCurrentItem(item);
		inputTab->setCurrentIndex(listWidget->currentRow());
		curEdit->setFocus();

		horizonUi.actionSave->setEnabled(true);
		horizonUi.actionSave_All->setEnabled(true);
		horizonUi.actionClose->setEnabled(true);
		horizonUi.actionUndo->setEnabled(true);
		horizonUi.actionRedo->setEnabled(true);
		horizonUi.actionCut->setEnabled(true);
		horizonUi.actionCopy->setEnabled(true);
		horizonUi.actionPaste->setEnabled(true);
		horizonUi.actionSelect_All->setEnabled(true);
		horizonUi.actionRun->setEnabled(true);
		horizonUi.actionBreak->setEnabled(true);
		horizonUi.actionCancelBreak->setEnabled(true);

		horizonUi.statusBar->showMessage(fi.fileName() + QString::fromLocal8Bit("新建至") + filePath, 3000);
	}
}
/*
读文件
*/
void Horizon::fOpen()
{
	QString filePath = QFileDialog::getOpenFileName(this,
		QString::fromLocal8Bit("打开文件"), productPath, tr("Txt or CMM (*.txt *.cmm)"));
	if (filePath != NULL && !file_paths.contains(filePath, Qt::CaseSensitive)) {
		file_paths += filePath;
		QFileInfo fi = QFileInfo(filePath);
		file_names += fi.fileName();
		//将选择的文件内容读取并呈现
		QFile file(filePath);
		if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
			return;
		QTextStream in(&file); 
		in.setCodec(QTextCodec::codecForName("utf-8"));
		QString all = in.readAll();
		SmartEdit* curEdit = new SmartEdit();
		curEdit->setPlainText(all);
		file.close();
		editors->append(curEdit);

		QListWidgetItem* item = new QListWidgetItem();
		item->setText(fi.fileName());
		listWidget->addItem(item);
		inputTab->addTab(curEdit, fi.fileName());
		listWidget->setCurrentItem(item);
		inputTab->setCurrentIndex(listWidget->currentRow());
		//<commit5>查看res\tools\commit.txt
		curEdit->setFocus();

		horizonUi.actionSave->setEnabled(true);
		horizonUi.actionSave_All->setEnabled(true);
		horizonUi.actionClose->setEnabled(true);
		horizonUi.actionUndo->setEnabled(true);
		horizonUi.actionRedo->setEnabled(true);
		horizonUi.actionCut->setEnabled(true);
		horizonUi.actionCopy->setEnabled(true);
		horizonUi.actionPaste->setEnabled(true);
		horizonUi.actionSelect_All->setEnabled(true);
		horizonUi.actionRun->setEnabled(true);
		horizonUi.actionBreak->setEnabled(true);
		horizonUi.actionCancelBreak->setEnabled(true);

		horizonUi.statusBar->showMessage(fi.fileName()
			+ QString::fromLocal8Bit("打开（F1加断点，F2取消断点）"), 5000);
	} else if (file_paths.contains(filePath, Qt::CaseSensitive)) {
		int index = file_paths.indexOf(filePath);
		listWidget->setCurrentRow(index);
		inputTab->setCurrentIndex(index);
		editors->at(index)->setFocus();
		//editors->at(index)->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
		horizonUi.statusBar->showMessage(QString::fromLocal8Bit("已在列表"), 3000);
	}
}


/*
保存当前文件并返回tab数量
*/
int Horizon::fSave()
{
	if (tabNotEmpty()) {
		int index = inputTab->currentIndex();
		QString filePath = file_paths.at(index);
		QFile file(filePath);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) 
			return inputTab->count();
		QTextStream out(&file);
		out.setCodec(QTextCodec::codecForName("utf-8"));
		QString text = editors->at(index)->toPlainText();
		out << text;
		file.close(); 
		horizonUi.statusBar->showMessage(QString::fromLocal8Bit("成功保存至") + filePath, 3000);
	}
	return inputTab->count();
}


/*
保存所有已打开文件
*/
void Horizon::fSave_All()
{
	if (tabNotEmpty()) {
		int fileCounts = file_paths.count();
		for (int i = 0; i < fileCounts; i++) {
			QFile file(file_paths.at(i));
			if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) 
				return;
			QTextStream out(&file);
			out.setCodec(QTextCodec::codecForName("utf-8"));
			out << editors->at(i)->toPlainText();
			file.close();
		}
		horizonUi.statusBar->showMessage(QString::fromLocal8Bit("全部保存成功"), 3000);
	}
}

/*根据文本是否改变提示保存并
关闭文件 tab/list/name/path/editors */
void Horizon::fClose()
{
	if (tabNotEmpty()) {
		int index = inputTab->currentIndex();
		QString filePath = file_paths.at(index);
		QFile file(filePath);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
		QTextStream in(&file);
		in.setCodec(QTextCodec::codecForName("utf-8"));
		QString all = in.readAll();
		file.close();
		QString text = editors->at(index)->toPlainText();
		int choose = -1;
		if (QString::compare(all, text) != 0) {
			choose = QMessageBox::question(this
				, QString::fromLocal8Bit("关闭当前文件")
				, QString::fromLocal8Bit("是否保存当前文件？")
				, QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
			if (QMessageBox::Yes == choose)  fSave(); 
		}
		if ( choose != QMessageBox::Cancel) {
			file_names.removeAt(index);
			file_paths.removeAt(index);
			editors->removeAt(index);
			//<commit2>请查看res\tools\commit.txt
			inputTab->removeTab(index);
			delete listWidget->takeItem(index);
		}
		if (!tabNotEmpty()) {
			horizonUi.actionSave->setEnabled(false);
			horizonUi.actionSave_All->setEnabled(false);
			horizonUi.actionClose->setEnabled(false);
			horizonUi.actionUndo->setEnabled(false);
			horizonUi.actionRedo->setEnabled(false);
			horizonUi.actionCut->setEnabled(false);
			horizonUi.actionCopy->setEnabled(false);
			horizonUi.actionPaste->setEnabled(false);
			horizonUi.actionSelect_All->setEnabled(false);
			horizonUi.actionRun->setEnabled(false);
			horizonUi.actionBreak->setEnabled(false);
			horizonUi.actionCancelBreak->setEnabled(false);
			horizonUi.actionLineDebug->setEnabled(false);
			horizonUi.actionPointDebug->setEnabled(false);
			consoleView->clear();
			IEView->clear();
		}
	}
}
/*无效行*/
bool Horizon::lineInvalid(QString curText) {
	return (NULL == curText || "{" == curText || "}" == curText || "$" == curText || curText.startsWith("/"));
}
/*给当前行添加断点*/
void Horizon::fBreak() {
	if (tabNotEmpty()) {
		SmartEdit* curEdit = editors->at(inputTab->currentIndex());
		int curLine = curEdit->textCursor().blockNumber(); 
		QString curText = curEdit->document()->findBlockByNumber(curLine).text().trimmed();
		if (-1 == curEdit->exceptionRow
			&& !curEdit->pointBlockNumbers.contains(curLine)
			&& !lineInvalid(curText)) {
			curEdit->pointBlockNumbers.append(curLine);
			horizonUi.actionPointDebug->setEnabled(true);
			horizonUi.actionLineDebug->setEnabled(true);
		}
	}
}
/*取消当前行断点*/
void Horizon::fCancelBreak() {
	if (tabNotEmpty()) {
		SmartEdit* curEdit = editors->at(inputTab->currentIndex());
		int curLine = curEdit->textCursor().blockNumber(); 
		if (curEdit->pointBlockNumbers.contains(curLine))
			curEdit->pointBlockNumbers.removeOne(curLine);
		if (curEdit->pointBlockNumbers.isEmpty()) {
			horizonUi.actionPointDebug->setEnabled(false);
			horizonUi.actionLineDebug->setEnabled(false);
		}
	}
}

/*逐句执行*/
void Horizon::fLineDebug() {
	if (tabNotEmpty()) {
		int index = inputTab->currentIndex();
		SmartEdit* curEdit = editors->at(index);
		if (!curEdit->pointBlockNumbers.isEmpty()) {
			if (curEdit->isEnabled()) {
				curEdit->setEnabled(false);
				horizonUi.actionRun->setEnabled(false);
				horizonUi.actionCancelDebug->setEnabled(true);
				fSave();
				vars = ReadVal(fTest());
			}
			int blockNumber = Step(vars);
			if (-1 == blockNumber) {
				curEdit->setEnabled(true);
				horizonUi.actionCancelDebug->setEnabled(false);
				horizonUi.actionRun->setEnabled(true);
			} else {
				curEdit->setTextCursor(QTextCursor(curEdit->document()->findBlockByNumber(blockNumber)));
			}
		}
	}

}
/*单步向后寻行*/
int Horizon::Step(QList<Var*>* vars)
{
	if (!vars->empty()) {
		Var* temp = vars->at(0);
		if (temp->Msg != NULL && temp->Msg != "WRITE") {
			consoleView->append(temp->Msg + "!	" + QString::fromLocal8Bit("行号:") + temp->Line);
			outputTab->setCurrentIndex(0);
			return -1;
		}
		int line = temp->Line.toInt();
		IEView->clear();
		IEView->append(QString::fromLocal8Bit("行号") + temp->Line + ":");
		while (line == temp->Line.toInt()) {
			if (temp->Msg == "WRITE") {
				consoleView->append(temp->Val);
				outputTab->setCurrentIndex(0);
			}
			else {
				IEView->append(QString::fromLocal8Bit("名:") + temp->Name + QString::fromLocal8Bit(" 类型:") +
					temp->Type + QString::fromLocal8Bit(" 值:") + temp->Val);
				outputTab->setCurrentIndex(1);
			}
			vars->removeFirst();
			if (!vars->empty())
				temp = vars->at(0);
			else
				return -1;
		}
		int blockNumber = line - 1;
		return blockNumber;
	} else {
		return -1;
	}
}


/*断点执行*/
void Horizon::fPointDebug() {
	if (tabNotEmpty()) {
		int index = inputTab->currentIndex();
		SmartEdit* curEdit = editors->at(index);
		if (!curEdit->pointBlockNumbers.isEmpty()) {
			if (curEdit->isEnabled()) {
				curEdit->setEnabled(false);
				horizonUi.actionRun->setEnabled(false);
				horizonUi.actionCancelDebug->setEnabled(true);
				fSave();
				vars = ReadVal(fTest());
			}
			int blockNumber = FindFirstPoint(vars, curEdit->pointBlockNumbers);
			if (-1 == blockNumber) {
				curEdit->setEnabled(true);
				horizonUi.actionCancelDebug->setEnabled(false);
				horizonUi.actionRun->setEnabled(true);
			} else {
				curEdit->setTextCursor(QTextCursor(curEdit->document()->findBlockByNumber(blockNumber)));
			}
		}
	}
}
/*找到接下来最近的一个断点行，成功输出断点行号，否则输出-1*/
int Horizon::FindFirstPoint(QList<Var*>* vars, QList<int> pointBlockNumbers)
{
	if (!vars->empty()) {
		//断点块号转断点行号
		QList<int> pointLines;
		for (int i = 0; i < pointBlockNumbers.count(); i++) {
			pointLines.append(pointBlockNumbers.at(i) + 1);
		}
		//末尾入行
		pointLines.append(vars->last()->Line.toInt());
		Var* temp = vars->at(0);
		int line = 0;
		while (!pointLines.contains(temp->Line.toInt())) {
			if (temp->Msg == "WRITE") {
				consoleView->append(temp->Val);
				outputTab->setCurrentIndex(0);
			}
			if (temp->Msg != NULL && temp->Msg != "WRITE") {
				consoleView->append(temp->Msg + "!	" + QString::fromLocal8Bit("行号:") + temp->Line);
				outputTab->setCurrentIndex(0);
				return -1;
			}
			vars->removeFirst();
			if (!vars->empty())
				temp = vars->at(0);
			else
				return -1;
		}
		line = temp->Line.toInt();
		if (temp) {
			if (temp->Msg != "WRITE") {
				IEView->clear();
				IEView->append(QString::fromLocal8Bit("行号") + temp->Line + ":");
				while (line == temp->Line.toInt()) {
					if (temp->Msg == "WRITE") {
						consoleView->append(temp->Val);
						outputTab->setCurrentIndex(0);
					}
					else {
						IEView->append(QString::fromLocal8Bit("名:") + temp->Name + QString::fromLocal8Bit(" 类型:") +
							temp->Type + QString::fromLocal8Bit(" 值:") + temp->Val);
						outputTab->setCurrentIndex(1);
					}
					vars->removeFirst();
					if (!vars->empty())
						temp = vars->at(0);
					else
						return -1;
				}
			}
			else {
				consoleView->append(temp->Val);
				outputTab->setCurrentIndex(0);
			}
		}
		else
			return -1;
		int blockNumber = line - 1;
		return blockNumber;
	}
	else {
		return -1;
	}
	
}

/*外部exe调用测试*/
QString Horizon::fTest() {//<commit6>
	QString standardOutput = NULL;
	if (tabNotEmpty()) {
		QProcess* process = new QProcess();
		//process->setProcessChannelMode(QProcess::MergedChannels);
		process->start("\"E:\\LLVM\\llvm build\\x64\\Debug\\Parser.exe\"", QStringList(file_paths.at(inputTab->currentIndex())));
		//process->start("\"E:\\LLVM\\llvm build\\x64\\Debug\\Parser.exe\"");
		qDebug()<<process->waitForStarted();
		process->waitForReadyRead();
		process->waitForFinished();
		while (!process->atEnd()) {
			standardOutput += QString::fromLocal8Bit(process->readLine().trimmed()) + " ";
		}
		if (process)
			process->close();
	}
	return standardOutput;
}

/*读取后台数据并规范化载入内存*/
QList<Var*>* Horizon::ReadVal(QString out)
{
	QString temp = NULL;
	QString line = NULL;
	QChar tcr = NULL;
	QList<Var*>* vars = new QList<Var*>();
	Var* tempVar;
	int n = 0;
	QString::const_iterator cit = NULL;//QChar指针，用来遍历QString
	for (cit = out.cbegin(); cit < out.cend(); cit++) {
		tcr = *cit;
		if (tcr == '@')
			n = 0;
		else if (tcr == ' ' || cit == out.cend()) {
			switch (n)
			{
			case 0:
				if (temp != 'W') {
					line = temp;
					temp.clear();
					n = 1;
				}
				else {
					temp.clear();
					n = 5;
				}
				break;
			case 1:
				tempVar = new Var();
				tempVar->Line = line;
				switch (temp.toInt())
				{
				case 0:tempVar->Type = "int"; break;
				case 1:tempVar->Type = "real"; break;
				case 2:tempVar->Type = "int[]"; break;
				case 3:tempVar->Type = "real[]"; break;
				default:
					break;
				}
				temp.clear();
				n = 2;
				break;
			case 2:
				tempVar->Name = temp;
				temp.clear();
				n = 3;
				break;
			case 3:
				tempVar->Val = temp;
				vars->append(tempVar);
				temp.clear();
				n = 1;
				break;
			case 4:
				tempVar = new Var();
				tempVar->Line = line;
				tempVar->Msg = temp;
				vars->append(tempVar);
				temp.clear();
				n = 1;
				break;
			case 5:
				tempVar = new Var();
				tempVar->Line = line;
				tempVar->Val = temp;
				tempVar->Msg = "WRITE";
				vars->append(tempVar);
				temp.clear();
				n = 1;
				break;
			default:
				break;
			}
		}
		else {
			temp += tcr;
			if (temp.contains("ERROR:")) {
				n = 4;
			}
		}
	}
	return vars;
}



void Horizon::fUndo() {
	if(tabNotEmpty()) editors->at(inputTab->currentIndex()) ->undo();
}
void Horizon::fRedo() {
	if (tabNotEmpty()) editors->at(inputTab->currentIndex())->redo();
}
void Horizon::fCut() {
	if (tabNotEmpty()) editors->at(inputTab->currentIndex())->cut();
}
void Horizon::fCopy() {
	if (tabNotEmpty()) editors->at(inputTab->currentIndex())->copy();
}
void Horizon::fPaste() {
	if (tabNotEmpty()) editors->at(inputTab->currentIndex())->paste();
}
void Horizon::fSelectAll() {
	if (tabNotEmpty()) editors->at(inputTab->currentIndex())->selectAll();
}

