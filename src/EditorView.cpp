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
		_filename = QString("new%1").arg(automatic_name++);
		_existingfilename = false;
	}
	else
	{
		QFileInfo file(filename);
		if(file.exists())
		{
			_existingfilename = true;
			_filename = file.filePath();
			editor->load(_filename);
		}
	}


	connect(editor, SIGNAL(textChanged()), this, SLOT(slotTextChanged()));
}


void EditorView::save()
{
	if(!_existingfilename)
	{
		saveAs();
		return;
	}

	if(!editor->save(_filename))
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

	_filename = filename;

	if(!_existingfilename)
	{
		_existingfilename = true;
		emit hasExistingFilename(true);
	}

	save();
}


QString EditorView::filename()
{
	return _filename;
}


bool EditorView::isExistingfilename()
{
	return _existingfilename;
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
	QFileInfo fi(_filename);
	return fi.name() + ((_changed) ? " *" : "");
}
