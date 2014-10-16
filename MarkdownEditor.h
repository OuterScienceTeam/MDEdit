#ifndef MARKDOWNEDITOR_H
#define MARKDOWNEDITOR_H

#include <QWidget>
#include <QContextMenuEvent>
#include <QPlainTextEdit>


class LineNumberWidget;

class MarkdownEditor : public QPlainTextEdit
{
	friend class LineNumberWidget;

	Q_OBJECT
public:
	explicit MarkdownEditor(QWidget* parent = 0);

protected:
	void contextMenuEvent(QContextMenuEvent* event);
};

#endif // MARKDOWNEDITOR_H
