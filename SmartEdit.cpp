#include "stdafx.h"
#include "SmartEdit.h"

HSynHighlighter::HSynHighlighter(QTextDocument *document)
	:QSyntaxHighlighter(document){}
//��ȡ��Ԫ�����жϹؼ��ֳ�ʲô��ɫ
void HSynHighlighter::readSynHighlighter(const QString &fileName){
    QFile file(fileName);
    if (false == file.open(QIODevice::ReadOnly)){
        qDebug() << "Open file " << fileName << "error" << __FUNCTION__;
    }

    QTextStream readFileStream(&file);
    QString curLine;//��ǰ��
	QStringList lineWords;//ÿ�еĴ�
	QString keyWords;//ÿ�е�һ�����ǹؼ���
    QRegularExpression spacesPattern("[ ]+");//commit1

    while(!readFileStream.atEnd()){
        curLine = readFileStream.readLine();
        if (curLine.startsWith("$$")) { continue; }//$$��Ϊ��ʼ��
        curLine.replace("\t", " ");//����ת�л�Ϊ�ո�
        lineWords = curLine.split(spacesPattern);//���дʰ��ո񻮷֣�����ո�ȼ���һ���ո�
        if (lineWords.size() != 4){ continue; }
        keyWords.append(lineWords.at(0)+"|");
        markedWordColorCase.insert(lineWords.at(0)//�ؼ���
			,QColor(lineWords.at(1).toInt()//r
				, lineWords.at(2).toInt()//g
				, lineWords.at(3).toInt()//b
			));
    }
    hKeyWordRegExp.setPattern(keyWords.trimmed());
}

//������ʾ�Ĺؼ��֣��飩
void HSynHighlighter::highlightBlock(const QString &text){
    QTextCharFormat hTextCharFormat;
    hTextCharFormat.setFontWeight(QFont::Bold);
    QRegularExpressionMatchIterator keyWordIterator = hKeyWordRegExp.globalMatch(text);
    while (keyWordIterator.hasNext()){
        QRegularExpressionMatch matchedWord = keyWordIterator.next();
        hTextCharFormat.setForeground(QBrush(markedWordColorCase.value(matchedWord.captured())));
        setFormat(matchedWord.capturedStart(), matchedWord.capturedLength(), hTextCharFormat);
    }
}

/**************SmartEdit******************/
//public functions
SmartEdit::SmartEdit(QTabWidget *parent):QPlainTextEdit(parent)
, hSynHighlighter(new HSynHighlighter(this->document()))
, keyWordsCompleter(NULL)
, lineNumberArea(new LineNumberArea(this))
, exceptionRow(-1) {
	
    hSynHighlighter->readSynHighlighter(HighLightCasesTxt); //�����﷨�����ļ�
    //��ȫ�б�����
    QMap<QString, QColor>::iterator iter;
    for (iter = hSynHighlighter->markedWordColorCase.begin(); 
		iter != hSynHighlighter->markedWordColorCase.end(); ++iter){
        keyWordsList.append(iter.key());
    }
	keyWordsCompleter = new QCompleter(keyWordsList);
	keyWordsCompleter->setWidget(this);
    keyWordsCompleter->setCaseSensitivity(Qt::CaseSensitive);//���ִ�Сд
    keyWordsCompleter->setCompletionMode(QCompleter::PopupCompletion);//ƥ������������,����
	keyWordsCompleter->popup()->setFont(QFont("΢���ź�", 12));
    keyWordsCompleter->setMaxVisibleItems(5);

	lineNumberArea->setFont(QFont("΢���ź�", 14, QFont::Bold)); //lineNumberArea->setVisible(true);

    connect(keyWordsCompleter, SIGNAL(activated(QString)), this, SLOT(onCompleterActivated(QString)));
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightIELines()));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(onCursorPosChanged()));

	setFont(QFont("΢���ź�",14));
	setWordWrapMode(QTextOption::NoWrap);  //ˮƽ����Ӧ������
	updateLineNumberAreaWidth(0);
    highlightCurrentLine();//������ʾ��ǰ��
}

SmartEdit::~SmartEdit(){
	delete hSynHighlighter;
	delete keyWordsCompleter;
	delete lineNumberArea;
}

#include <QByteArray>//�˴�λ�ò��ɵ�ǰ

QString SmartEdit::keyWordAtCursor() const{
    //���������ƶ�cursor����ѡ���ַ������鿴ѡ�еĵ������Ƿ��пո񡪡��ո���Ϊ���ʵķָ���
    QTextCursor curTextCursor = textCursor();
    QString selectedString;
    while (curTextCursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, 1)){
        selectedString = curTextCursor.selectedText();
        if (selectedString.startsWith(QString(" ")) || 
			selectedString.startsWith(QChar(0x422029))){ break; }
    }
    if (selectedString.startsWith(QChar(0x422029))){
        selectedString.replace(0, 1, QChar(' '));
    }return selectedString.trimmed();
}

int SmartEdit::lineNumberAreaWidth(){
    int digits = 4;//��ʼλ�����
    int max = qMax(1, blockCount());//��ȡ���λ��
    while (max >= 100){//2λ�����ȹ̶���3λ������������������
        max /= 10;
        ++digits;
    }
	//����Ӧ����lineNumberAreaһ��
	int space = lineNumberArea->fontMetrics().width(QChar('0')) * digits;
    return space;
}

//protected Events
void SmartEdit::keyPressEvent(QKeyEvent *e){
    if (keyWordsCompleter){
        if (keyWordsCompleter->popup()->isVisible()){
            switch(e->key()){
                case Qt::Key_Escape:
                case Qt::Key_Enter:
                case Qt::Key_Return:
                case Qt::Key_Tab:
					e->ignore(); return;
				default:break;
            }
        }
        QPlainTextEdit::keyPressEvent(e);
        completionPrefix = this->keyWordAtCursor();
        keyWordsCompleter->setCompletionPrefix(completionPrefix); // ͨ������QCompleter��ǰ׺������CompleterѰ�ҹؼ���
        curTextCursorRect = cursorRect();
        if (completionPrefix == ""){ return; }
        if (keyWordsCompleter->completionCount() > 0){
            keyWordsCompleter->complete(
				QRect(curTextCursorRect.left()+lineNumberAreaWidth(), curTextCursorRect.top()+fontMetrics().height(), 150, 1));
		}else{
			keyWordsCompleter->popup()->setVisible(false);
		}
    }
	
}

void SmartEdit::keyReleaseEvent(QKeyEvent* e) {
	QPlainTextEdit::keyReleaseEvent(e);
	int curLine = textCursor().blockNumber();
	switch ( e->key()) {
	case Qt::Key_ParenLeft:
		this->textCursor().insertText(")");
		textCursor().movePosition(QTextCursor::Left,QTextCursor::KeepAnchor,1);
		textCursor().clearSelection(); break;
	case  Qt::Key_BracketLeft:
		this->textCursor().insertText("]"); break;
	case  Qt::Key_BraceLeft:
		this->textCursor().insertText("}"); break;
	case Qt::Key_F1:
		if(!IELines.contains(curLine))
			IELines.append(curLine); break;
	case Qt::Key_F2:
		if (IELines.contains(curLine))
			IELines.removeOne(curLine); break;
	default: break;
	}
}

void SmartEdit::resizeEvent(QResizeEvent *e){
    QPlainTextEdit::resizeEvent(e);
    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void SmartEdit::lineNumberAreaPaintEvent(QPaintEvent* event) {
    QPainter painter(lineNumberArea);
	QTextBlock block = firstVisibleBlock();
	painter.fillRect(event->rect(), QColor(220, 220, 220));
	//painter.setBrush(Qt::red);
	int blockNumber = block.blockNumber()
		, top = (int)blockBoundingGeometry(block).translated(contentOffset()).top()//ֱ�Ӹ�ֵΪ0���кź��ı��߶Ȳ�����
		, bottom = top + (int)blockBoundingRect(block).height();//�������Ϊ�����Ҳ�ᵼ���к�������߶Ȳ�����
	int numberHeight = fontMetrics().height(), breakpointR = numberHeight - 4;
    while (block.isValid() && top <= event->rect().bottom()){
        if (block.isVisible() && bottom >= event->rect().top()) {
			if (exceptionRow == blockNumber + 1) {
				painter.fillRect(QRect(0, top, lineNumberArea->width(), numberHeight), QColor(Qt::red).lighter(128));
				painter.setBrush(Qt::white);
			} else {
				painter.setBrush(QColor(Qt::red).lighter(128));
			}
			if (IELines.contains(blockNumber)) {
				painter.drawEllipse(3, top+2, breakpointR, breakpointR);
			}
			QString number = QString::number(blockNumber + 1);
			painter.drawText(0, top, lineNumberArea->width(), numberHeight, Qt::AlignRight, number);
			this->update();
        }
		block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        blockNumber++;
    }
}

//public slots
void SmartEdit::onCompleterActivated(const QString &completion){
    QString curPrefix = keyWordAtCursor(),
            shouldInsertText = completion;
    curTextCursor = textCursor();
    if (!completion.contains(curPrefix)){  // ɾ��֮ǰδ��ȫ���ַ�
        curTextCursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, curPrefix.size());
        curTextCursor.clearSelection();
    } else {  // ��ȫ��Ӧ���ַ�
        shouldInsertText = shouldInsertText.replace(
            shouldInsertText.indexOf(curPrefix), curPrefix.size(), "");
    }curTextCursor.insertText(shouldInsertText);
}

void SmartEdit::onCursorPosChanged(){
    QString completionPrefix = keyWordAtCursor();
    if (completionPrefix == ""){
        keyWordsCompleter->setCompletionPrefix("----");
        keyWordsCompleter->complete(QRect(curTextCursorRect.left(), curTextCursorRect.top(), 0, 0));
    }highlightCurrentLine();
}

void SmartEdit::updateLineNumberAreaWidth(int /* newBlockCount */){
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void SmartEdit::highlightCurrentLine(){
    QList<QTextEdit::ExtraSelection> extraSelections;
    if (!isReadOnly()){
        QTextEdit::ExtraSelection selection;
        selection.format.setBackground(QColor(Qt::yellow).lighter(160));//���õ�ǰ�б���ɫ
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }
	setExtraSelections(extraSelections);
}

void SmartEdit::updateLineNumberArea(const QRect & rect, int dy){
    if (dy) lineNumberArea->scroll(0, dy);
    else  lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
	if (rect.contains(viewport()->rect())) updateLineNumberAreaWidth(0);
}


