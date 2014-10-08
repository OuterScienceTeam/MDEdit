#include "EditorView.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>


int EditorView::automatic_name = 1;


EditorView::EditorView(QString filename)
{
	_changed = false;

	QHBoxLayout* layout = new QHBoxLayout(this);

	editor = new MarkdownEditor(QFont("DejaVu Sans Mono"), this);
	layout->addWidget(editor);

	this->setLayout(layout);


	if(filename.isEmpty())
	{
		_file.setFile(QString("new%1").arg(automatic_name++));
	}
	else
	{
		_file.setFile(filename);
		if(_file.exists())
		{
			// assure absolute path
			QString absolutePath = _file.absoluteFilePath();
			_file.setFile(absolutePath);
			editor->load(absolutePath);
		}
	}


	connect(editor, SIGNAL(textChanged()), this, SLOT(slotTextChanged()));
}


void EditorView::save()
{
	if(!_file.exists())
	{
		saveAs();
		return;
	}

	if(!editor->save(_file.absoluteFilePath()))
		return;

	if(_changed)
	{
		_changed = false;
		emit changed(false);
	}
}


void EditorView::saveAs()
{
	QString filename = QFileDialog::getSaveFileName(this->window(), "Save As", "", "Markdown (*.md *.markdown)");
	if(filename.isEmpty())
		return;

	bool exists = _file.exists();

	_file.setFile(filename);
	// assure absolute path
	QString absolutePath = _file.absoluteFilePath();
	_file.setFile(absolutePath);

	if(!editor->save(absolutePath))
		return;

	if(_changed || !exists)
	{
		_changed = false;
		emit changed(false);
	}

	if(!exists)
	{
		emit hasExistingFilename(true);
	}

	emit filenameChanged();
}


QString EditorView::filename()
{
	return _file.fileName();
}


QString EditorView::fullFilename()
{
	return isExistingfilename() ? _file.absoluteFilePath() : filename();
}


bool EditorView::isExistingfilename()
{
	return _file.exists();
}


bool EditorView::isChanged()
{
	return _changed;
}


void EditorView::slotTextChanged()
{
	if(!_changed)
	{
		_changed = true;
		emit changed(true);
	}
}


QString EditorView::tabLabel()
{
	return _file.fileName() + ((_changed) ? " *" : "");
}
