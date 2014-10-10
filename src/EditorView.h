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

	bool readFile();
	bool writeFile();

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

	bool isUndoAvailable() const;
	bool isRedoAvailable() const;

signals:
	void modificationChanged(bool changed);
	void filenameChanged();
	void changed();
	void cursorPositionChanged(int line, int col);
	void undoAvailable(bool available);
	void redoAvailable(bool available);

private slots:
	void slotModificationChanged(bool modified);
	void slotTextChanged();
	void slotCursorPositionChanged();
	void slotUndoAvailable(bool available);
	void slotRedoAvailable(bool available);

public slots:
	void save();
	void saveAs();
	void exportHtml();
	void undo();
	void redo();
};

#endif // EDITORVIEW_H
