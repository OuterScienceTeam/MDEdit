#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include <QWidget>
#include <QFileInfo>

#include "MarkdownEditor.h"

class EditorView : public QWidget
{
	Q_OBJECT
private:
	static int automatic_name;

	QFileInfo _file;
	bool _changed;

	MarkdownEditor* editor;

public:
	explicit EditorView(QString filename);

	QString filename();
	bool isExistingfilename();
	bool isChanged();

	QString tabLabel();

signals:
	void changed(bool changed);
	void hasExistingFilename(bool hasExistingFilename);

private slots:
	void slotTextChanged();

public slots:
	void save();
	void saveAs();
};

#endif // EDITORVIEW_H
