#pragma once
#ifndef SCALEWIDGET_H
#define SCALEWIDGET_H

#include "SmartEdit.h"
#include <QtWidgets/QWidget>

enum CursorStyle {
	CursorNormal = 0    // 普通鼠标
	, CursorRight        // 水平右拉伸
	, CursorLeft          //水平左拉伸
	, CursorBottom           // 垂直底拉伸
	, CursorTop                //垂直顶拉伸
	, CursorDiagBR        // 右下角拉伸
	, CursorDiagTR           //右上角拉伸
	, CursorDiagBL         //左下角拉伸
	, CursorDiagTL         //左上角拉伸
};

class ScaleWidget : public QWidget{
	Q_OBJECT

public:
	ScaleWidget();
	~ScaleWidget();
	QWidget* inner;

private:
	int setCursorStyle(const QPoint& curPoint);    // 当前位置设置鼠标样式
	void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;//override
	void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;//override
	void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;//override

	QPoint lastPos;  //记录窗口上次的位置
	QPoint m_pressedPos_inResizableArea;   // 记录鼠标在可拉伸区按下时的坐标
	bool  m_isPressed_inResizableArea;  // 鼠标是否在拉伸区按下
	int  m_cursorStyle;     // 光标类型
	int  resizableAreaWidth;         //可拉伸区域宽度
	int resizableAreaWidth_duo;
};

#endif // SCALEWIDGET_H