#include "stdafx.h"
#include "HTextEditor.h"

HSynHighlighter::HSynHighlighter(QTextDocument *document)
	:QSyntaxHighlighter(document)
{
}

//读取二元表，以判断关键字呈什么颜色
void HSynHighlighter::readSynHighlighter(const QString &fileName)
{
    QFile file(fileName);
    if (false == file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Open file " << fileName << "error" << __FUNCTION__;
    }

    QTextStream readFileStream(&file);
    QString keyWord;
    QString readLineStr;
    QStringList lineWordList;
    QString markWordCases;
    QRegularExpression regString("[ ]+");
    int r, g, b;

    while(!readFileStream.atEnd())
    {
        readLineStr = readFileStream.readLine();
        if (readLineStr.startsWith("$$")) { continue; }
        readLineStr.replace("\t", " ");
        lineWordList = readLineStr.split(regString);
        if (lineWordList.size() != 4){ continue; }
        keyWord = lineWordList.at(0);
        markWordCases.append(keyWord);
        markWordCases.append("|");
        r = lineWordList.at(1).toInt();
        g = lineWordList.at(2).toInt();
        b = lineWordList.at(3).toInt();
        markedWordColorCase.insert(keyWord, QColor(r, g, b));
    }
    markWordCases.trimmed();
    HRegExpression.setPattern(markWordCases);
    qDebug() << "markWordCases:" << markWordCases;
}

//高亮显示
void HSynHighlighter::highlightBlock(const QString &text)
{
    QTextCharFormat hTextCharFormat;
    hTextCharFormat.setFontWeight(QFont::Bold);
    QRegularExpressionMatchIterator Iterator = HRegExpression.globalMatch(text);
    while (Iterator.hasNext())
    {
        QRegularExpressionMatch match = Iterator.next();
        hTextCharFormat.setForeground(QBrush(markedWordColorCase.value(match.captured())));
        setFormat(match.capturedStart(), match.capturedLength(), hTextCharFormat);
    }
}

/**************HTextEditor******************/
//public functions
HTextEditor::HTextEditor(QWidget *parent):QPlainTextEdit(parent)
{
    hSynHighlighter = new HSynHighlighter(this->document());
    hSynHighlighter->readSynHighlighter(QString("markWordColorCase/C.txt")); //设置语法高亮文件

    //补全列表设置
    QMap<QString, QColor>::iterator iter;
    for (iter = hSynHighlighter->markedWordColorCase.begin(); iter != hSynHighlighter->markedWordColorCase.end(); ++iter)
    {
        keyWordsList.append(iter.key());
    }

    keyWordsComplter = new QCompleter(keyWordsList);
	keyWordsComplter->setWidget(this);
    keyWordsComplter->setCaseSensitivity(Qt::CaseInsensitive);//不区分大小写
    keyWordsComplter->setCompletionMode(QCompleter::PopupCompletion);//匹配已输入内容弹出
	keyWordsComplter->popup()->setFont(QFont("微软雅黑", 14));
	keyWordsComplter->popup()->setBackgroundRole(QPalette::ColorRole::ToolTipText);
    keyWordsComplter->setMaxVisibleItems(5);

    lineNumberArea = new LineNumberArea(this);
    lineNumberArea->setVisible(true);

    connect(keyWordsComplter, SIGNAL(activated(QString)), this, SLOT(onCompleterActivated(QString)));

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));

    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(onCurosPosChange()));


	setFont(QFont("微软雅黑",18));
    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

}
#include <QByteArray>

QString HTextEditor::wordUnderCursor() const
{
    //不断向左移动cursor，并选中字符，并查看选中的单词中是否含有空格——空格作为单词的分隔符
    QTextCursor curTextCursor = textCursor();
    QString selectedString;
    while (curTextCursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, 1))
    {
        selectedString = curTextCursor.selectedText();
        if (selectedString.startsWith(QString(" ")) || selectedString.startsWith(QChar(0x422029)))
        {
            break;
        }

    }
    if (selectedString.startsWith(QChar(0x422029)))
    {
        selectedString.replace(0, 1, QChar(' '));
    }
    return selectedString.trimmed();
	
}

int HTextEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10)
    {
        max /= 10;
        ++digits;
    }
    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;
    return space;
}

//protected Events
void HTextEditor::keyPressEvent(QKeyEvent *e)
{
    if (keyWordsComplter)
    {
        if (keyWordsComplter->popup()->isVisible())
        {
            switch(e->key())
            {
                case Qt::Key_Escape:
                case Qt::Key_Enter:
                case Qt::Key_Return:
                case Qt::Key_Tab:
                    e->ignore();
                    return;
                default:
                    break;
            }
        }
        QPlainTextEdit::keyPressEvent(e);
        completerPrefix = this->wordUnderCursor();
        keyWordsComplter->setCompletionPrefix(completerPrefix); // 通过设置QCompleter的前缀，来让Completer寻找关键词
        curTextCursorRect = cursorRect();
        if (completerPrefix == "")
        {
            return;
        }
        if (keyWordsComplter->completionCount() > 0)
        {
            keyWordsComplter->complete(QRect(curTextCursorRect.left(), curTextCursorRect.top(), 200, 15));
		}
		else
		{
			keyWordsComplter->popup()->setVisible(false);
		}
    }
	switch (e->key())
	{
	case Qt::Key_ParenLeft:

	default:
		break;
	}
}

void HTextEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);
    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void HTextEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

//public slots
void HTextEditor::onCompleterActivated(const QString &completion)
{
    QString completionPrefix = wordUnderCursor(),
            shouldInertText = completion;
    curTextCursor = textCursor();
    if (!completion.contains(completionPrefix))
    {
        // delete the previously typed.
        curTextCursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, completionPrefix.size());
        curTextCursor.clearSelection();
    }
    else
    {
        // 补全相应的字符
        shouldInertText = shouldInertText.replace(
            shouldInertText.indexOf(completionPrefix), completionPrefix.size(), "");
    }
    curTextCursor.insertText(shouldInertText);
}

void HTextEditor::onCurosPosChange()
{
    QString completionPrefix = wordUnderCursor();
    if (completionPrefix == "")
    {
        keyWordsComplter->setCompletionPrefix("----");
        keyWordsComplter->complete(QRect(curTextCursorRect.left(), curTextCursorRect.top(), 0, 0));
    }
    highlightCurrentLine();

}

void HTextEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void HTextEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void HTextEditor::updateLineNumberArea(const QRect & rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void HTextEditor::appendText(const QString &text)
{
	this->appendPlainText(text);
}
