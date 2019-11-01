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
	QSplitter* horizontalSplitter; //ˮƽ�ָ�������ֱ��
	QSplitter* verticalSplitter;//��ֱ�ָ�����ˮƽ��
	QListWidget* listWidget;//�б�չʾ�ļ�
	QList<SmartEdit*>* editors;//�Ѵ򿪵ı༭��
	QStringList file_names;//�Ѵ򿪵��ļ�
	QStringList file_paths;//��Ӧ·��
	QTabWidget* tabWidget;//�л�ͼ��չʾ�༭��
	SmartEdit* inputView;//�༭�������
	QTextEdit* outputView; //�����
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
