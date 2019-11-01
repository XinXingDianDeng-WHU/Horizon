#pragma once
#ifndef HTEXTEDITOR_H
#define HTEXTEDITOR_H
#include "ScaleWidget.h"
#include "Parser/MyParser.h"
#include <QtWidgets>

//词色二元表
class HSynHighlighter : public QSyntaxHighlighter {
	Q_OBJECT
public:
	QMap<QString, QColor> markedWordColorCase; //保存语法高亮信息
	HSynHighlighter(QTextDocument* document = 0);
	void readSynHighlighter(const QString& fileName);

protected:
	void highlightBlock(const QString& text);

private:
	QRegularExpression hKeyWordRegExp;//以“|”为划分的关键字
};

//开发输入模块
class SmartEdit : public QPlainTextEdit {
	Q_OBJECT
public:
	SmartEdit(QTabWidget* parent = 0);
	~SmartEdit();
	int exceptionRow;
	QString keyWordAtCursor() const;
	int lineNumberAreaWidth();
	void lineNumberAreaPaintEvent(QPaintEvent* event);

protected:
	void resizeEvent(QResizeEvent* event) override;
	void keyPressEvent(QKeyEvent* e);
	void keyReleaseEvent(QKeyEvent* e);

public slots:
	void onCompleterActivated(const QString& completion);
	void onCursorPosChanged(void);

private slots:
	void updateLineNumberAreaWidth(int newBlockCount);
	void updateLineNumberArea(const QRect&, int);
	void highlightCurrentLine();

private:
	HSynHighlighter* hSynHighlighter;
	QCompleter* keyWordsCompleter;
	QWidget* lineNumberArea; //显示行号
	QStringList keyWordsList;
	QTextCursor curTextCursor;
	QRect curTextCursorRect;
	QString completionPrefix;
	const QString HighLightCasesTxt = ".//res//tools//HighLightCases.txt";

};

//行号模块, 必须放在SmartEdit类的后面，否则编译无法通过
class LineNumberArea : public QWidget {
public:
	explicit LineNumberArea(SmartEdit* editor) : QWidget(editor) {
		hSmartEdit = editor; //setVisible(true);
	}
	QSize sizeHint() const override {
		return QSize(hSmartEdit->lineNumberAreaWidth(), 0);
	}

protected:
	void paintEvent(QPaintEvent* event) override {
		hSmartEdit->lineNumberAreaPaintEvent(event);
	}

private:
	SmartEdit* hSmartEdit;
};

#endif // HTEXTEDITOR_H

