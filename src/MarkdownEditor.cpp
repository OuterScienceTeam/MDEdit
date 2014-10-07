#include "MarkdownEditor.h"

#include <QTextBlock>
#include <QMenu>
#include <QChar>

#include <QFile>
#include <QTextStream>
#include <QStringBuilder>
#include <QDebug>


MarkdownEditor::MarkdownEditor(QFont font, QWidget *parent)
	: QPlainTextEdit(parent)
{
	setFont(font);
}


bool MarkdownEditor::load(QString &filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	QTextStream input(&file);

	this->setPlainText(input.readAll());

	return true;
}


bool MarkdownEditor::save(QString &filename)
{
	//TODO saving files
	return false;
}


void MarkdownEditor::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu* menu = createStandardContextMenu();
	//TODO custom menu items
	menu->exec(event->globalPos());
	delete menu;
}
