#include "stdafx.h"
#include "ScaleWidget.h"

//可拉伸构件
ScaleWidget::ScaleWidget()
	: QWidget()
	, inner(new QWidget(this))
	//, edi(new SmartEdit(tab))
	, m_isPressed_inResizableArea(false)
	, m_cursorStyle(CursorNormal)
	, resizableAreaWidth(6){
	resizableAreaWidth_duo = resizableAreaWidth * 2;
	setWindowFlags(Qt::FramelessWindowHint);    // 设置无边框无标志无按钮
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);    // 设置尺寸属性
	setMouseTracking(true);    // 跟踪鼠标，界面拉伸需要这个属性
	setMinimumSize(200, 100);    // 设置最小尺寸
	setStyleSheet("background-color:purple;");
	inner->setStyleSheet("background-color:darkCyan;");
	inner->move(resizableAreaWidth, resizableAreaWidth);
	inner->resize(this->width() - resizableAreaWidth_duo, this->height() - resizableAreaWidth_duo);
}
//析构
ScaleWidget::~ScaleWidget(){}


//鼠标按键
void ScaleWidget::mousePressEvent(QMouseEvent* event){
	//如果光标不是默认光标，说明鼠标是在拉伸区点击的
	if (CursorNormal != m_cursorStyle) {
		m_isPressed_inResizableArea = true;
		m_pressedPos_inResizableArea = event->pos();
	} event->ignore();    //表示继续向下传递事件，其他的控件还可以去获取
	//鼠标相关的每个事件都采取ignore，可不断刷新鼠标状态
}

//鼠标松键，数据复位
void ScaleWidget::mouseReleaseEvent(QMouseEvent* event){
	if (event->button() == Qt::LeftButton){
		m_isPressed_inResizableArea = false;
		m_pressedPos_inResizableArea.setX(-1);
		m_pressedPos_inResizableArea.setY(-1);
		m_cursorStyle = setCursorStyle(event->pos());
	} event->ignore();
}

//鼠标移动
void ScaleWidget::mouseMoveEvent(QMouseEvent* event){
	//记录鼠标当前坐标
	QPoint curPoint = event->pos();
	//如果不在可拉伸区，则刷新
	if (!m_isPressed_inResizableArea) {
		m_cursorStyle = setCursorStyle(curPoint);
	}

	//以下注释用以测试
	/*setToolTipDuration(30000);
	setToolTip("thisX:" + QString::number(this->x())
		+ "\nthisW:" + QString::number(this->width())
		+ "\nPosX:" + QString::number(curPoint.x())
		+ "\nPosY:" + QString::number(curPoint.y()));*/
		//测试发现，resize函数参数是相对于窗口坐标的，即相对坐标
		//而move函数参数是相对鱼整个屏幕的，即绝对坐标
	//注意光标在窗口左外部时，其x坐标为负值

	//如果在可拉伸区，则执行相应拉伸动作
	if (m_isPressed_inResizableArea && !m_pressedPos_inResizableArea.isNull()) {
		switch (m_cursorStyle) {
		case CursorLeft://左
			if (curPoint.x() + this->minimumWidth() <= this->width()) {
				this->move(this->x() + curPoint.x(), this->y());
				this->resize(this->width() - curPoint.x(), this->height());
			}break;
		case CursorRight://右
			this->resize(curPoint.x(), this->height());break;
		case CursorTop://顶
			if (curPoint.y() + this->minimumHeight() <= this->height()) {
				this->move(this->x(), this->y() + curPoint.y());
				this->resize(this->width(), this->height() - curPoint.y());
			}break;
		case CursorBottom://底
			this->resize(this->width(), curPoint.y());break;
		case CursorDiagBR://右下
			this->resize(curPoint.x(), curPoint.y());break;
		case CursorDiagTR://右上
			if (curPoint.y() + this->minimumHeight() <= this->height()) {
				this->move(this->x(), this->y() + curPoint.y());
				this->resize(curPoint.x(), this->height() - curPoint.y());
			}break;
		case CursorDiagBL://左下
			if (curPoint.x() + this->minimumWidth() <= this->width()) {
				this->move(this->x() + curPoint.x(), this->y());
				this->resize(this->width() - curPoint.x(), curPoint.y());
			}break;
		case CursorDiagTL://左上
			if (curPoint.x() + this->minimumWidth() <= this->width()
				&& curPoint.y() + this->minimumHeight() <= this->height()) {
				this->move(this->x() + curPoint.x(), this->y() + curPoint.y());
				this->resize(this->width() - curPoint.x(), this->height() - curPoint.y());
			}break;
		default:break;
		}
	}
	if(inner !=NULL)inner->resize(this->width() - resizableAreaWidth_duo, this->height() - resizableAreaWidth_duo);
	event->ignore();
}

int ScaleWidget::setCursorStyle(const QPoint& curPoint){
	int nCurWidth = this->width();
	int nCurHeight = this->height();
	int nRes = CursorNormal;

	//鼠标光标变水平拉伸、垂直拉伸、角拉伸
	//角拉伸附属水平拉伸、垂直拉伸均可，此处附属水平拉伸
	if (curPoint.x() >= 0 && curPoint.x() < resizableAreaWidth) {
		setCursor(Qt::SizeHorCursor); nRes = CursorLeft;//左
		if (curPoint.y() >= 0 && curPoint.y() < resizableAreaWidth) {
			setCursor(Qt::SizeFDiagCursor); nRes = CursorDiagTL;//左上
		} else if (curPoint.y() <= nCurHeight && curPoint.y() > nCurHeight - resizableAreaWidth) {
			setCursor(Qt::SizeBDiagCursor); nRes = CursorDiagBL;//左下
		}return nRes;
	} else if (curPoint.x() <= nCurWidth && curPoint.x() > nCurWidth - resizableAreaWidth) {
		setCursor(Qt::SizeHorCursor); nRes = CursorRight;//右
		if (curPoint.y() >= 0 && curPoint.y() < resizableAreaWidth) {
			setCursor(Qt::SizeBDiagCursor); nRes = CursorDiagTR;//右上
		} else if (curPoint.y() <= nCurHeight && curPoint.y() > nCurHeight - resizableAreaWidth) {
			setCursor(Qt::SizeFDiagCursor); nRes = CursorDiagBR;//右下
		}return nRes;
	} else if(curPoint.y() <= nCurHeight && curPoint.y() > nCurHeight - resizableAreaWidth){ //底
		setCursor(Qt::SizeVerCursor); nRes = CursorBottom; return nRes;
	} else if (curPoint.y() >= 0 && curPoint.y() < resizableAreaWidth) { //顶
		setCursor(Qt::SizeVerCursor); nRes = CursorTop; return nRes;
	} else { //普通光标
		setCursor(Qt::ArrowCursor); return nRes;
	}
}


