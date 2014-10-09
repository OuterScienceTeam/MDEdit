#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include <QWidget>
#include <QFileInfo>

#include "MarkdownEditor.h"
#include "LineNumberWidget.h"

class EditorView : public QWidget
{
	Q_OBJECT
private:
	static int automatic_name;

	QFileInfo _file;
	bool _virtual;
	bool _changed;

	MarkdownEditor* editor;
	LineNumberWidget* lineNumberWidget;

public:
	EditorView(QString filename);

	QString filename() const;
	QString fullFilename() const;
	bool isVirtual() const;
	bool isChanged() const;
	int length() const;

	QString tabLabel() const;

	QString getHtml() const;

signals:
	void changed(bool changed);
	void filenameChanged();
	void changed();
	void cursorPositionChanged(int line, int col);

private slots:
	void slotTextChanged();
	void slotCursorPositionChanged();

public slots:
	void save();
	void saveAs();
	void exportHtml();
};

#endif // EDITORVIEW_H
