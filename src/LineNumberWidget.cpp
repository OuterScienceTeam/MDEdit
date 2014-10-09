#include "LineNumberWidget.h"

#include <QScrollBar>
#include <QPainter>
#include <QTextBlock>


LineNumberWidget::LineNumberWidget(MarkdownEditor* editor, QWidget *parent) :
	QWidget(parent)
{
	_editor = editor;

	setFixedWidth(_editor->fontMetrics().width("00000") + 20);

	connect(_editor->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(repaint()));
	connect(_editor, SIGNAL(cursorPositionChanged()), this, SLOT(repaint()));
	connect(_editor, SIGNAL(textChanged()), this, SLOT(repaint()));
}

void LineNumberWidget::paintEvent(QPaintEvent* /*event*/)
{
	QPainter painter(this);

	painter.eraseRect(contentsRect());


	painter.setFont(_editor->font());
	painter.setPen(QColor("#626262"));
	painter.setRenderHint(QPainter::Antialiasing);

	const QFontMetrics fm = _editor->fontMetrics();

	int lineNumber = 1;
	QTextBlock currentBlock = _editor->document()->begin();
	QTextBlock nextBlock;
	QString lineNubmerString;
	int top = -1;
	int lineHeight = fm.lineSpacing();
	int textWidth;

	while(currentBlock.isValid())
	{
		if(currentBlock.isVisible())
		{
			top = (int) _editor->blockBoundingGeometry(currentBlock).translated(_editor->contentOffset()).top();
		}

		nextBlock = currentBlock.next();

		if (top < 0)
		{
			currentBlock = currentBlock.next();
			lineNumber++;
			continue;
		}

		if (top > height())
			break;

		lineNubmerString = QString::number(lineNumber);
		textWidth = fm.width(lineNubmerString);

		painter.drawText(width() - 10 - textWidth, top, textWidth, lineHeight, Qt::AlignRight | Qt::AlignTop, lineNubmerString);

		currentBlock = currentBlock.next();

		lineNumber++;
	}

	painter.end();
}
