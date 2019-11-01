#pragma once

#include <QMainWindow>
#include "ui_Horizon.h"
#include "SmartEdit.h"
#include "newFileDialog.h"
#include "Parser/MyParser.h"

namespace Ui {
	class Horizon;
}

class Horizon : public QMainWindow{
	Q_OBJECT

public:
	Horizon(QMainWindow *parent = Q_NULLPTR);
	~Horizon(); 
	QLabel* curDateTimeWidget;
	QTimer* timer;

public slots:
	void showCurDateTime();
	void inputViewSyn_Tab(int index);
	void inputViewSyn_List(int index);
	void fUndo();
	void fRedo();
	void fCut();
	void fCopy();
	void fPaste();
	void fSelectAll();

protected:
	void closeEvent(QCloseEvent* event)override;

private:
	Ui::HorizonClass horizonUi;
	QSplitter* horizontalSplitter; //水平分割器，垂直向
	QSplitter* verticalSplitter;//垂直分割器，水平向
	QListWidget* listWidget;//列表，展示文件
	QList<SmartEdit*>* editors;//已打开的编辑框
	QStringList file_names;//已打开的文件
	QStringList file_paths;//对应路径
	QTabWidget* tabWidget;//切换图，展示编辑框
	SmartEdit* inputView;//编辑框、输入框
	QTextEdit* outputView; //输出框
	MyParser* parser;
	const char* SLR1Txt = ".//res//tools//SLR1.txt"
		, * MyProductions = ".//res//tools//MyProductions.txt";
	const QString productPath = ".//res//product//";

private slots:
	int fQuit();
	int fSave();
	void fNew();
	void fOpen();
	void fSave_All();
	void fClose();
	void fSafeRun();
	void fStaticSemanticAnalysis();
};
