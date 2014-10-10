#include "MarkdownEditor.h"

#include <QMenu>
#include <QChar>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

#include "MarkdownParser.h"


MarkdownEditor::MarkdownEditor(QFont font, QWidget *parent)
	: QPlainTextEdit(parent)
{
	setFont(font);
	setContentsMargins(0, 0, 0, 0);
	setFrameStyle(QFrame::NoFrame);
	setFrameShadow(QFrame::Plain);
	setWordWrapMode(QTextOption::NoWrap);
	setTabStopWidth(fontMetrics().width(QLatin1Literal("0000")));
}


bool MarkdownEditor::load(const QString& filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::critical(this->window(), "Failed to open file", "File \"" + filename + "\" could not be opened for reading. Please check whether you have read permission to the file.");
		return false;
	}

	QTextStream input(&file);

	this->setPlainText(input.readAll());

	file.close();

	return true;
}


bool MarkdownEditor::save(const QString& filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly))
	{
		QMessageBox::critical(this->window(), "Error opening file", "File \"" + filename + "\" could not be opened for writing. Please check whether you have write permission to the file.");
		return false;
	}

	QTextStream output(&file);
	output.setGenerateByteOrderMark(false);
	output.setCodec("UTF-8");
	output << toPlainText();
	file.close();

	return true;
}


QString MarkdownEditor::getHtml()
{
	return parseMarkdownPage(toPlainText());
}


void MarkdownEditor::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu* menu = createStandardContextMenu();
	//TODO custom menu items
	menu->exec(event->globalPos());
	delete menu;
}
