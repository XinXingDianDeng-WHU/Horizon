#pragma once

#include <QMainWindow>
#include "ui_Horizon.h"
#include "SmartEdit.h"
#include "newFileDialog.h"

class Horizon : public QMainWindow{
	Q_OBJECT

public:
	Horizon(QMainWindow *parent = Q_NULLPTR);
	~Horizon();

private:
	Ui::HorizonClass ui;
	QSplitter* horizontalSplitter; //水平分割器，垂直向
	QSplitter* verticalSplitter;//垂直分割器，水平向
	QListWidget* listWidget;//列表，展示文件
	QList<SmartEdit*>* editors;//已打开的编辑框
	QStringList file_names;//已打开的文件
	QStringList file_paths;//对应路径
	QTabWidget* tabWidget;//切换图，展示编辑框
	SmartEdit* inputView;//编辑框、输入框
	QTextEdit* outputView; //输出框


private slots:
	void fNew();
	void fOpen();
	void fSave();
	void fSave_As();
	void fSave_All();
	void item_changed();
	void fClose();
	//void eidtResize();
};
