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
	QSplitter* horizontalSplitter; //ˮƽ�ָ�������ֱ��
	QSplitter* verticalSplitter;//��ֱ�ָ�����ˮƽ��
	QListWidget* listWidget;//�б�չʾ�ļ�
	QList<SmartEdit*>* editors;//�Ѵ򿪵ı༭��
	QStringList file_names;//�Ѵ򿪵��ļ�
	QStringList file_paths;//��Ӧ·��
	QTabWidget* tabWidget;//�л�ͼ��չʾ�༭��
	SmartEdit* inputView;//�༭�������
	QTextEdit* outputView; //�����


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
