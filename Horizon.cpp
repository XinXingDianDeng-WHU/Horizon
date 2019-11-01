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
	, outputView(new QTextEdit(verticalSplitter))
	, curDateTimeWidget(new QLabel())
	, timer(new QTimer(this))
	, parser(NULL) {

	horizonUi.setupUi(this); 
	timer->start(1000);
	horizontalSplitter->addWidget(listWidget); 
	horizontalSplitter->addWidget(verticalSplitter);
	verticalSplitter->addWidget(tabWidget); 
	outputView->setReadOnly(true);
	verticalSplitter->addWidget(outputView);
	verticalSplitter->setStretchFactor(0, 1);//<commit0>��鿴res\tools\commit.txt
	horizontalSplitter->setStretchFactor(1, 1);
	this->setCentralWidget(horizontalSplitter);
	this->setMinimumSize(1200, 540);
	curDateTimeWidget->setAlignment(Qt::AlignRight);
	horizonUi.statusBar->addPermanentWidget(curDateTimeWidget);
	horizonUi.statusBar->showMessage(QString::fromLocal8Bit("Horizon ��л����ʹ��"), 6000);

	connect(horizonUi.actionQuit, SIGNAL(triggered()), this, SLOT(fQuit()));
	connect(horizonUi.actionNew, SIGNAL(triggered()), this, SLOT(fNew()));
	connect(horizonUi.actionOpen, SIGNAL(triggered()), this, SLOT(fOpen()));
	connect(horizonUi.actionSave, SIGNAL(triggered()), this, SLOT(fSave()));
	connect(horizonUi.actionSave_All, SIGNAL(triggered()), this, SLOT(fSave_All()));
	connect(horizonUi.actionClose, SIGNAL(triggered()), this, SLOT(fClose()));
	connect(horizonUi.actionRun, SIGNAL(triggered()), this, SLOT(fSafeRun()));
	connect(horizonUi.actionUndo, SIGNAL(triggered()), this, SLOT(fUndo()));
	connect(horizonUi.actionRedo, SIGNAL(triggered()), this, SLOT(fRedo()));
	connect(horizonUi.actionCut, SIGNAL(triggered()), this, SLOT(fCut()));
	connect(horizonUi.actionCopy, SIGNAL(triggered()), this, SLOT(fCopy()));
	connect(horizonUi.actionPaste, SIGNAL(triggered()), this, SLOT(fPaste()));
	connect(horizonUi.actionSelect_All, SIGNAL(triggered()), this, SLOT(fSelectAll()));
	connect(tabWidget, SIGNAL(tabBarClicked(int)), this, SLOT(inputViewSyn_Tab(int)));
	connect(listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(inputViewSyn_List(int)));
	connect(timer, SIGNAL(timeout()), this, SLOT(showCurDateTime()));
	connect(timer, SIGNAL(timeout()), this, SLOT(fStaticSemanticAnalysis()));
}
Horizon::~Horizon() {}

/*��ȡ��ǰʱ��*/
void Horizon::showCurDateTime() {
	curDateTimeWidget->setText(QDate::currentDate().toString()
		+ " / " + QTime::currentTime().toString() + " ");
}

/*�˵���ť�˳�*/
int Horizon::fQuit() {
	int choose = -1;
	if (tabWidget->count() > 0) {
		choose = QMessageBox::question(this
			, QString::fromLocal8Bit("�˳�")
			, QString::fromLocal8Bit("�Ƿ񱣴������ļ���")
			, QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		if (QMessageBox::Yes == choose)  fSave_All();
		if (choose != QMessageBox::Cancel) qApp->quit();
	} else {
		qApp->quit();
	}
	return choose;
}

/*���ڰ�ť�˳�*/
void Horizon::closeEvent(QCloseEvent* event) {//<commit3>��鿴res\tools\commit.txt
	if (QMessageBox::Cancel == fQuit())
		event->ignore();
}
/*
����
*/
void Horizon::fSafeRun() {
	if (fSave() > 0) {
		try {
			parser = new MyParser(file_paths.at(tabWidget->currentIndex()).toStdString().c_str()
				, SLR1Txt, MyProductions, "empty", true);
			parser->Parse(); 
			outputView->setReadOnly(false);
			outputView->append(QTime::currentTime().toString()
				+ " <" + file_names.at(tabWidget->currentIndex()) + "> "
				+ QString::fromLocal8Bit("�﷨�����;�̬�������ͨ��"));
			outputView->setReadOnly(true);
		}
		catch (Exception e) {
			//��ת��������,�����кű�����ʵ���кŶ�Ӧ����block�����Ǵ�0��ʼ�����e.row=blockNumber+1
			inputView->setTextCursor(QTextCursor(inputView->document()->findBlockByNumber(e.row-1)));
			outputView->setReadOnly(false);
			outputView->append(QTime::currentTime().toString()
				+ " <" + file_names.at(tabWidget->currentIndex()) + "> "
				+ QString::fromLocal8Bit(e.print().c_str()));
			outputView->setReadOnly(true);
		}
		horizonUi.statusBar->showMessage(QString::fromLocal8Bit("�������"), 3000);
	}
}

/*inputView��tab��editorsͬ��*/
void Horizon::inputViewSyn_Tab(int index) {
	if (index >= 0) { inputView = editors->at(index); }
}

/*inputView��Tab��list��editorsͬ��*/
void Horizon::inputViewSyn_List(int index) {
	if (index >= 0) { //����漰connect������û�д����Խ�����⣬��������������
		tabWidget->setCurrentIndex(index); 
		inputView = editors->at(tabWidget->currentIndex());
	}
}

/*��̬�������*/
void Horizon::fStaticSemanticAnalysis() {
	//����150�в�����ִ�о�̬�������
	if (inputView && inputView->document()->lineCount() <= 150) {
		QString curText = inputView->toPlainText() + '\0';
		try {
			parser = new MyParser(curText.toStdString().c_str()
				, SLR1Txt, MyProductions, "empty", false);
			parser->Parse();
			inputView->exceptionRow = -1;
		}
		catch(Exception e){
			inputView->exceptionRow = e.row;
		}
	}
}

//�½��ļ���path/name/editors/list/tab
void Horizon::fNew()
{
	newFileDialog* dialog = new newFileDialog(this);
	dialog->exec();
	QString filePath = dialog->new_file; 
	if (filePath != NULL) {
		QFile file(filePath);
		if (!file.open(QIODevice::NewOnly | QIODevice::ReadWrite)) return;
		file_paths += filePath;
		QFileInfo fi = QFileInfo(filePath);
		file_names += fi.fileName();
		//��ѡ����ļ����ݶ�ȡ������
		QTextStream in(&file);
		in.setCodec(QTextCodec::codecForName("utf-8"));
		QString line = in.readAll();
		inputView = new SmartEdit();
		inputView->appendPlainText(line);
		file.close();
		editors->append(inputView);

		QListWidgetItem* item = new QListWidgetItem();
		item->setText(fi.fileName());
		listWidget->addItem(item);
		tabWidget->addTab(inputView, fi.fileName());
		//<commit4>��鿴res\tools\commit.txt
		listWidget->setCurrentItem(item);
		tabWidget->setCurrentIndex(listWidget->currentRow());
		inputView->setFocus();
		horizonUi.statusBar->showMessage(fi.fileName() + QString::fromLocal8Bit("�½���") + filePath, 3000);
	}
}
/*
���ļ�
*/
void Horizon::fOpen()
{
	QString filePath = QFileDialog::getOpenFileName(this,
		QString::fromLocal8Bit("���ļ�"), productPath, tr("Txt or CMM (*.txt *.cmm)"));
	if (filePath != NULL && !file_paths.contains(filePath, Qt::CaseSensitive)) {
		file_paths += filePath;
		QFileInfo fi = QFileInfo(filePath);
		file_names += fi.fileName();
		//��ѡ����ļ����ݶ�ȡ������
		QFile file(filePath);
		if (!file.open(QIODevice::ReadOnly)) return;
		QTextStream in(&file); 
		in.setCodec(QTextCodec::codecForName("utf-8"));
		QString all = in.readAll();
		inputView = new SmartEdit();
		inputView->setPlainText(all);
		file.close();
		editors->append(inputView);

		QListWidgetItem* item = new QListWidgetItem();
		item->setText(fi.fileName());
		listWidget->addItem(item);
		tabWidget->addTab(inputView, fi.fileName());
		listWidget->setCurrentItem(item);
		tabWidget->setCurrentIndex(listWidget->currentRow());
		//<commit5>�鿴res\tools\commit.txt
		inputView->setFocus();
		inputView->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
		horizonUi.statusBar->showMessage(fi.fileName()
			+ QString::fromLocal8Bit("��"), 3000);
	} else if (file_paths.contains(filePath, Qt::CaseSensitive)) {
		int index = file_paths.indexOf(filePath);
		listWidget->setCurrentRow(index);
		tabWidget->setCurrentIndex(index);
		inputView = editors->at(index);
		inputView->setFocus();
		inputView->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
		horizonUi.statusBar->showMessage(QString::fromLocal8Bit("�����б�"), 3000);
	}
}


/*
���浱ǰ�ļ�������tab����
*/
int Horizon::fSave()
{
	if (tabWidget->count()>0) {
		int index = tabWidget->currentIndex();
		QString filePath = file_paths.at(index);
		QFile file(filePath);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return tabWidget->count();
		QTextStream out(&file);
		out.setCodec(QTextCodec::codecForName("utf-8"));
		QString text = editors->at(index)->toPlainText();
		out << text;
		file.close(); 
		horizonUi.statusBar->showMessage(QString::fromLocal8Bit("�ɹ�������") + filePath, 3000);
	}
	return tabWidget->count();
}


/*
���������Ѵ��ļ�
*/
void Horizon::fSave_All()
{
	if (tabWidget->count() > 0) {
		int fileCounts = file_paths.count();
		for (int i = 0; i < fileCounts; i++) {
			QFile file(file_paths.at(i));
			if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
			QTextStream out(&file);
			out.setCodec(QTextCodec::codecForName("utf-8"));
			out << editors->at(i)->toPlainText();
			file.close();
		}
		horizonUi.statusBar->showMessage(QString::fromLocal8Bit("ȫ������ɹ�"), 3000);
	}
}

/*�ر��ļ� tab/list/name/path/editors */
void Horizon::fClose()
{
	if (tabWidget->count() > 0) {
		int index = tabWidget->currentIndex();
		QString filePath = file_paths.at(index);
		QFile file(filePath);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) 
			return;
		QTextStream in(&file);
		in.setCodec(QTextCodec::codecForName("utf-8"));
		QString all = in.readAll();		
		file.close();
		QString text = editors->at(index)->toPlainText();
		int choose = -1;
		if (QString::compare(all, text) != 0) {			
			choose = QMessageBox::question(this
				, QString::fromLocal8Bit("�رյ�ǰ�ļ�")
				, QString::fromLocal8Bit("�Ƿ񱣴浱ǰ�ļ���")
				, QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
			if (QMessageBox::Yes == choose) { fSave(); }
			
		}
		if (QMessageBox::Cancel != choose) {
			file_names.removeAt(index);
			file_paths.removeAt(index);
			editors->removeAt(index);
			//<commit2>��鿴res\tools\commit.txt
			tabWidget->removeTab(index);
			delete listWidget->takeItem(index);

			if (0 == tabWidget->count()) {
				inputView = NULL; outputView->clear();
			}
			else {
				inputView = editors->at(tabWidget->currentIndex());
			}
		}
		
	}
}

void Horizon::fUndo() {
	if (inputView)inputView->undo();
}
void Horizon::fRedo() {
	if (inputView)inputView->redo();
}
void Horizon::fCut() {
	if (inputView)inputView->cut();
}
void Horizon::fCopy() {
	if (inputView)inputView->copy();
}
void Horizon::fPaste() {
	if (inputView)inputView->paste();
}
void Horizon::fSelectAll() {
	if (inputView)inputView->selectAll();
}

