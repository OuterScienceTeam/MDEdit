#include "EditorView.h"

#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>

#include "MarkdownParser.h"


int EditorView::automatic_name = 1;


EditorView::EditorView(QString filename)
{
	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setMargin(0);
	layout->setSpacing(0);

	editor = new MarkdownEditor(this);

	lineNumberWidget = new LineNumberWidget(editor, this);

	layout->addWidget(lineNumberWidget);
	layout->addWidget(editor);

	this->setLayout(layout);


	if(filename.isEmpty())
	{
		_virtual = true;
		_file.setFile(QString("new%1.md").arg(automatic_name++));
	}
	else
	{
		_virtual = false;
		_file.setFile(filename);
		_file.makeAbsolute();
		if(_file.exists())
		{
			readFile();
		}
	}


	connect(editor, SIGNAL(textChanged()), this, SLOT(slotTextChanged()));
	connect(editor, SIGNAL(cursorPositionChanged()), this, SLOT(slotCursorPositionChanged()));
	connect(editor->document(), SIGNAL(modificationChanged(bool)), this, SLOT(slotModificationChanged(bool)));
	connect(editor->document(), SIGNAL(undoAvailable(bool)), this, SLOT(slotUndoAvailable(bool)));
	connect(editor->document(), SIGNAL(redoAvailable(bool)), this, SLOT(slotRedoAvailable(bool)));
}


EditorView::~EditorView()
{
	delete editor;
	delete lineNumberWidget;
}


void EditorView::save()
{
	if(_virtual)
	{
		saveAs();
		return;
	}

	if(!writeFile())
		return;

	editor->document()->setModified(false);
}


void EditorView::saveAs()
{
	QString filename = QFileDialog::getSaveFileName(this->window(), "Save As", _file.filePath(), "Markdown (*.md *.markdown);;Hypertext Markup Language (*.html)");
	if(filename.isEmpty())
		return;

	if(filename.endsWith(".html"))
	{
		exportHtml(filename);
		return;
	}

	_file.setFile(filename);
	_file.makeAbsolute();

	if(!writeFile())
		return;

	_virtual = false;

	emit filenameChanged();
}


void EditorView::undo()
{
	editor->document()->undo();
}


void EditorView::redo()
{
	editor->document()->redo();
}


QString EditorView::filename() const
{
	return _file.fileName();
}


QString EditorView::fullFilename() const
{
	return isVirtual() ? filename() : _file.absoluteFilePath();
}


QString EditorView::dirname() const
{
	return _file.absolutePath();
}


bool EditorView::isVirtual() const
{
	return _virtual;
}


bool EditorView::isModified() const
{
	return editor->document()->isModified();
}


int EditorView::length() const
{
	return editor->toPlainText().length();
}


bool EditorView::isUndoAvailable() const
{
	return editor->document()->isUndoAvailable();
}


bool EditorView::isRedoAvailable() const
{
	return editor->document()->isRedoAvailable();
}


int EditorView::cursorLine() const
{
	return editor->textCursor().blockNumber();
}


int EditorView::cursorCol() const
{
	return editor->textCursor().columnNumber();
}


void EditorView::slotModificationChanged(bool modified)
{
	emit modificationChanged(modified);
}


void EditorView::slotTextChanged()
{
	emit changed();
}


void EditorView::slotCursorPositionChanged()
{
	emit cursorPositionChanged(editor->textCursor().blockNumber(), editor->textCursor().columnNumber());
}


void EditorView::slotRedoAvailable(bool available)
{
	emit redoAvailable(available);
}


void EditorView::slotUndoAvailable(bool available)
{
	emit undoAvailable(available);
}


QString EditorView::getHtml() const
{
	return parseMarkdownPage(editor->toPlainText());
}


QString EditorView::tabLabel() const
{
	return _file.fileName() + (isModified() ? " *" : "");
}


bool EditorView::readFile()
{
	QFile file(_file.absoluteFilePath());
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::critical(this->window(), "Failed to open file", "File \"" + file.fileName() + "\" could not be opened for reading. Please check whether you have read permission to the file.");

		qDebug() << "Failed to read" << file.fileName() << file.errorString();

		return false;
	}

	QTextStream input(&file);

	editor->blockSignals(true);
	editor->setPlainText(input.readAll());
	editor->document()->setModified(false);
	editor->blockSignals(false);

	file.close();

	qDebug() << "Read" << file.fileName();

	return true;
}


bool EditorView::writeFile()
{
	QFile file(_file.absoluteFilePath());
	if (!file.open(QIODevice::WriteOnly))
	{
		QMessageBox::critical(this->window(), "Error opening file", "File \"" + file.fileName() + "\" could not be opened for writing. Please check whether you have write permission to the file.");

		qDebug() << "Failed to save" << file.fileName() << file.errorString();

		return false;
	}

	QTextStream output(&file);
	output.setGenerateByteOrderMark(false);
	output.setCodec("UTF-8");
	output << editor->toPlainText();
	file.close();

	editor->document()->setModified(false);

	qDebug() << "Saved" << file.fileName();

	return true;
}


bool EditorView::exportHtml(const QString& filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly))
	{
		QMessageBox::critical(this->window(), "Failed to open file", "File \"" + filename + "\" could not be opened for writing.");

		qDebug() << "Could export HTML into" << filename << file.errorString();

		return false;
	}

	QTextStream output(&file);
	output.setCodec("UTF-8");
	output << getHtml();
	file.close();

	qDebug() << "File" << _file.filePath() << "exported as HTML" << filename;

	return true;
}
