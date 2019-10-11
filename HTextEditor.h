#pragma once
#ifndef SIMPLEEDITOR_H
#define SIMPLEEDITOR_H

#include <QtWidgets>
//颜色类型
enum highLightColorType
{
	Black,Gray,White,
	Red,Orange,Yellow,Green,
	Cyan,Blue,Purple
};

//QMap<QString, QColor>standardWordColorCase = {
//	{Black,QColor(0,0,0)},{Gray,QColor(190,190,190)},{White,QColor(255,255,255)},
//	{Red,QColor(255,0,0)},{Orange,QColor(255,165,0)},{Yellow,QColor(255,255,0)},{Green,QColor(0,255,0)},
//	{Cyan,QColor(0,255,255)},{Blue,QColor(0,0,255)},{Purple,QColor(160,32,240)}
//};
//词色二元结构体
typedef struct highLightWord {
	QString keyWord;
	QColor   highlightColor;
}markedWord;

//词色二元表
class HSynHighlighter : public QSyntaxHighlighter {
	Q_OBJECT
public:
	HSynHighlighter(QTextDocument* document = 0);
	void readSynHighlighter(const QString& fileName);
	QMap<QString, QColor> markedWordColorCase; //保存语法高亮信息

protected:
	void highlightBlock(const QString& text);

private:
	QRegularExpression HRegExpression;
};


//开发输入模块
class HTextEditor : public QPlainTextEdit {
	Q_OBJECT
public:
	HTextEditor(QWidget* parent = 0);

	QString wordUnderCursor() const;
	int lineNumberAreaWidth();
	void lineNumberAreaPaintEvent(QPaintEvent* event);
	void appendText(const QString &text);

protected:
	void resizeEvent(QResizeEvent* event) override;
	void keyPressEvent(QKeyEvent* e);

private slots:
	void updateLineNumberAreaWidth(int newBlockCount);
	void highlightCurrentLine();
	void updateLineNumberArea(const QRect&, int);

public slots:
	void onCompleterActivated(const QString& completion);
	void onCurosPosChange(void);

private:
	HSynHighlighter* hSynHighlighter;
	QCompleter* keyWordsComplter;
	QStringList keyWordsList;
	QTextCursor curTextCursor;
	QRect curTextCursorRect;
	QString completerPrefix;
	//显示行号
	QWidget* lineNumberArea;

};
//行号模块
class LineNumberArea : public QWidget
{
public:
	explicit LineNumberArea(HTextEditor* editor) : QWidget(editor)
	{
		hTextEdit = editor;
		//setVisible(true);
	}

	QSize sizeHint() const override
	{
		return QSize(hTextEdit->lineNumberAreaWidth(), 0);
	}

protected:
	void paintEvent(QPaintEvent* event) override
	{
		hTextEdit->lineNumberAreaPaintEvent(event);
		//qDebug() << "hTextEdit:" << __FUNCTION__;
	}

private:
	HTextEditor* hTextEdit;
};

#endif // SIMPLEEDITOR_H

