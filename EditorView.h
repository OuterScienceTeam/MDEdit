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
	bool exportHtml(const QString& filename);

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

	int cursorLine() const;
	int cursorCol() const;

	void save();
	void saveAs();
	void undo();
	void redo();

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
};

#endif // EDITORVIEW_H
