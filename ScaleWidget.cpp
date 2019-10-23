#include "stdafx.h"
#include "ScaleWidget.h"

//�����칹��
ScaleWidget::ScaleWidget()
	: QWidget()
	, inner(new QWidget(this))
	//, edi(new SmartEdit(tab))
	, m_isPressed_inResizableArea(false)
	, m_cursorStyle(CursorNormal)
	, resizableAreaWidth(6){
	resizableAreaWidth_duo = resizableAreaWidth * 2;
	setWindowFlags(Qt::FramelessWindowHint);    // �����ޱ߿��ޱ�־�ް�ť
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);    // ���óߴ�����
	setMouseTracking(true);    // ������꣬����������Ҫ�������
	setMinimumSize(200, 100);    // ������С�ߴ�
	setStyleSheet("background-color:purple;");
	inner->setStyleSheet("background-color:darkCyan;");
	inner->move(resizableAreaWidth, resizableAreaWidth);
	inner->resize(this->width() - resizableAreaWidth_duo, this->height() - resizableAreaWidth_duo);
}
//����
ScaleWidget::~ScaleWidget(){}


//��갴��
void ScaleWidget::mousePressEvent(QMouseEvent* event){
	//�����겻��Ĭ�Ϲ�꣬˵��������������������
	if (CursorNormal != m_cursorStyle) {
		m_isPressed_inResizableArea = true;
		m_pressedPos_inResizableArea = event->pos();
	} event->ignore();    //��ʾ�������´����¼��������Ŀؼ�������ȥ��ȡ
	//�����ص�ÿ���¼�����ȡignore���ɲ���ˢ�����״̬
}

//����ɼ������ݸ�λ
void ScaleWidget::mouseReleaseEvent(QMouseEvent* event){
	if (event->button() == Qt::LeftButton){
		m_isPressed_inResizableArea = false;
		m_pressedPos_inResizableArea.setX(-1);
		m_pressedPos_inResizableArea.setY(-1);
		m_cursorStyle = setCursorStyle(event->pos());
	} event->ignore();
}

//����ƶ�
void ScaleWidget::mouseMoveEvent(QMouseEvent* event){
	//��¼��굱ǰ����
	QPoint curPoint = event->pos();
	//������ڿ�����������ˢ��
	if (!m_isPressed_inResizableArea) {
		m_cursorStyle = setCursorStyle(curPoint);
	}

	//����ע�����Բ���
	/*setToolTipDuration(30000);
	setToolTip("thisX:" + QString::number(this->x())
		+ "\nthisW:" + QString::number(this->width())
		+ "\nPosX:" + QString::number(curPoint.x())
		+ "\nPosY:" + QString::number(curPoint.y()));*/
		//���Է��֣�resize��������������ڴ�������ģ����������
		//��move���������������������Ļ�ģ�����������
	//ע�����ڴ������ⲿʱ����x����Ϊ��ֵ

	//����ڿ�����������ִ����Ӧ���춯��
	if (m_isPressed_inResizableArea && !m_pressedPos_inResizableArea.isNull()) {
		switch (m_cursorStyle) {
		case CursorLeft://��
			if (curPoint.x() + this->minimumWidth() <= this->width()) {
				this->move(this->x() + curPoint.x(), this->y());
				this->resize(this->width() - curPoint.x(), this->height());
			}break;
		case CursorRight://��
			this->resize(curPoint.x(), this->height());break;
		case CursorTop://��
			if (curPoint.y() + this->minimumHeight() <= this->height()) {
				this->move(this->x(), this->y() + curPoint.y());
				this->resize(this->width(), this->height() - curPoint.y());
			}break;
		case CursorBottom://��
			this->resize(this->width(), curPoint.y());break;
		case CursorDiagBR://����
			this->resize(curPoint.x(), curPoint.y());break;
		case CursorDiagTR://����
			if (curPoint.y() + this->minimumHeight() <= this->height()) {
				this->move(this->x(), this->y() + curPoint.y());
				this->resize(curPoint.x(), this->height() - curPoint.y());
			}break;
		case CursorDiagBL://����
			if (curPoint.x() + this->minimumWidth() <= this->width()) {
				this->move(this->x() + curPoint.x(), this->y());
				this->resize(this->width() - curPoint.x(), curPoint.y());
			}break;
		case CursorDiagTL://����
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

	//������ˮƽ���졢��ֱ���졢������
	//�����츽��ˮƽ���졢��ֱ������ɣ��˴�����ˮƽ����
	if (curPoint.x() >= 0 && curPoint.x() < resizableAreaWidth) {
		setCursor(Qt::SizeHorCursor); nRes = CursorLeft;//��
		if (curPoint.y() >= 0 && curPoint.y() < resizableAreaWidth) {
			setCursor(Qt::SizeFDiagCursor); nRes = CursorDiagTL;//����
		} else if (curPoint.y() <= nCurHeight && curPoint.y() > nCurHeight - resizableAreaWidth) {
			setCursor(Qt::SizeBDiagCursor); nRes = CursorDiagBL;//����
		}return nRes;
	} else if (curPoint.x() <= nCurWidth && curPoint.x() > nCurWidth - resizableAreaWidth) {
		setCursor(Qt::SizeHorCursor); nRes = CursorRight;//��
		if (curPoint.y() >= 0 && curPoint.y() < resizableAreaWidth) {
			setCursor(Qt::SizeBDiagCursor); nRes = CursorDiagTR;//����
		} else if (curPoint.y() <= nCurHeight && curPoint.y() > nCurHeight - resizableAreaWidth) {
			setCursor(Qt::SizeFDiagCursor); nRes = CursorDiagBR;//����
		}return nRes;
	} else if(curPoint.y() <= nCurHeight && curPoint.y() > nCurHeight - resizableAreaWidth){ //��
		setCursor(Qt::SizeVerCursor); nRes = CursorBottom; return nRes;
	} else if (curPoint.y() >= 0 && curPoint.y() < resizableAreaWidth) { //��
		setCursor(Qt::SizeVerCursor); nRes = CursorTop; return nRes;
	} else { //��ͨ���
		setCursor(Qt::ArrowCursor); return nRes;
	}
}


