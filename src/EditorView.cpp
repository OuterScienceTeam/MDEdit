#include "EditorView.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>


int EditorView::automatic_name = 1;


EditorView::EditorView(QString filename)
{
	_changed = false;

	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setMargin(0);
	layout->setSpacing(0);

	editor = new MarkdownEditor(QFont("DejaVu Sans Mono"), this);
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
			editor->load(_file.absoluteFilePath());
		}
	}


	connect(editor, SIGNAL(textChanged()), this, SLOT(slotTextChanged()));
}


void EditorView::save()
{
	if(_virtual)
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

	_file.setFile(filename);
	_file.makeAbsolute();

	if(!editor->save(_file.absoluteFilePath()))
		return;

	if(_changed || _virtual)
	{
		_changed = false;
		emit changed(false);
	}

	_virtual = false;

	emit filenameChanged();
}


QString EditorView::filename() const
{
	return _file.fileName();
}


QString EditorView::fullFilename() const
{
	return isVirtual() ? _file.absoluteFilePath() : filename();
}


bool EditorView::isVirtual() const
{
	return _virtual;
}


bool EditorView::isChanged() const
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

	emit changed();
}


QString EditorView::getHtml() const
{
	return editor->getHtml();
}


QString EditorView::tabLabel() const
{
	return _file.fileName() + ((_changed) ? " *" : "");
}
