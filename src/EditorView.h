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

	MarkdownEditor* editor;
	LineNumberWidget* lineNumberWidget;

public:
	EditorView(QString filename);
	~EditorView();

	QString filename() const;
	QString fullFilename() const;
	bool isVirtual() const;
	bool isModified() const;
	int length() const;

	QString tabLabel() const;

	QString getHtml() const;

signals:
	void modificationChanged(bool changed);
	void filenameChanged();
	void changed();
	void cursorPositionChanged(int line, int col);

private slots:
	void slotModificationChanged(bool modified);
	void slotTextChanged();
	void slotCursorPositionChanged();

public slots:
	void save();
	void saveAs();
	void exportHtml();
};

#endif // EDITORVIEW_H
