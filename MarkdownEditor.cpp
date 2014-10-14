#include "MarkdownEditor.h"

#include <QMenu>

MarkdownEditor::MarkdownEditor(QWidget *parent)
	: QPlainTextEdit(parent)
{
	setContentsMargins(0, 0, 0, 0);
	setFrameStyle(QFrame::NoFrame);
	setFrameShadow(QFrame::Plain);
	setWordWrapMode(QTextOption::NoWrap);
	setTabStopWidth(fontMetrics().width(QLatin1Literal("0000")));
}


void MarkdownEditor::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu* menu = createStandardContextMenu();
	//TODO custom menu items
	menu->exec(event->globalPos());
	delete menu;
}
