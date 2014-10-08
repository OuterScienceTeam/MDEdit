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
	bool _virtual;
	bool _changed;

	MarkdownEditor* editor;

public:
	EditorView(QString filename);

	QString filename() const;
	QString fullFilename() const;
	bool isVirtual() const;
	bool isChanged() const;

	QString tabLabel() const;

	QString getHtml() const;

signals:
	void changed(bool changed);
	void filenameChanged();
	void changed();

private slots:
	void slotTextChanged();

public slots:
	void save();
	void saveAs();
	void exportHtml();
};

#endif // EDITORVIEW_H
