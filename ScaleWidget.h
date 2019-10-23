#pragma once
#ifndef SCALEWIDGET_H
#define SCALEWIDGET_H

#include "SmartEdit.h"
#include <QtWidgets/QWidget>

enum CursorStyle {
	CursorNormal = 0    // ��ͨ���
	, CursorRight        // ˮƽ������
	, CursorLeft          //ˮƽ������
	, CursorBottom           // ��ֱ������
	, CursorTop                //��ֱ������
	, CursorDiagBR        // ���½�����
	, CursorDiagTR           //���Ͻ�����
	, CursorDiagBL         //���½�����
	, CursorDiagTL         //���Ͻ�����
};

class ScaleWidget : public QWidget{
	Q_OBJECT

public:
	ScaleWidget();
	~ScaleWidget();
	QWidget* inner;

private:
	int setCursorStyle(const QPoint& curPoint);    // ��ǰλ�����������ʽ
	void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;//override
	void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;//override
	void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;//override

	QPoint lastPos;  //��¼�����ϴε�λ��
	QPoint m_pressedPos_inResizableArea;   // ��¼����ڿ�����������ʱ������
	bool  m_isPressed_inResizableArea;  // ����Ƿ�������������
	int  m_cursorStyle;     // �������
	int  resizableAreaWidth;         //������������
	int resizableAreaWidth_duo;
};

#endif // SCALEWIDGET_H