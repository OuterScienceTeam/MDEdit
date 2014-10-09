#ifndef MARKDOWNEDITOR_H
#define MARKDOWNEDITOR_H

#include <QWidget>
#include <QContextMenuEvent>
#include <QPlainTextEdit>


class LineNumberWidget;

class MarkdownEditor : public QPlainTextEdit
{
	Q_OBJECT
private:

public:
	explicit MarkdownEditor(QFont font, QWidget *parent = 0);

	bool load(const QString& filename);
	bool save(const QString& filename);

	QString getHtml();

protected:
	void contextMenuEvent(QContextMenuEvent* event);

	friend LineNumberWidget;
};

#endif // MARKDOWNEDITOR_H
