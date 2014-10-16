#ifndef LINENUMBERWIDGET_H
#define LINENUMBERWIDGET_H

#include <QWidget>

#include "MarkdownEditor.h"

class LineNumberWidget : public QWidget
{
	Q_OBJECT
private:
	MarkdownEditor* _editor;

public:
	LineNumberWidget(MarkdownEditor* editor, QWidget* parent = 0);

protected:
	void paintEvent(QPaintEvent* event);
};

#endif // LINENUMBERWIDGET_H
