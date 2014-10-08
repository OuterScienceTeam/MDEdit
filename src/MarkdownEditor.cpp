#include "MarkdownEditor.h"

#include <QTextBlock>
#include <QMenu>
#include <QChar>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QStringBuilder>
#include <QDebug>

#include "MarkdownParser.h"


static void toPlainText(QString& text)
{
	QChar* current = text.data();
	QChar* end = text.data() + text.size();
	for(; current != end; ++current)
	{
		switch(current->unicode())
		{
			case 0xfdd0: // QTextBeginningOfFrame
			case 0xfdd1: // QTextEndOfFrame
			case QChar::ParagraphSeparator:
			case QChar::LineSeparator:
				*current = QLatin1Char('\n');
				break;

			default:
				break;
		}
	}
}


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
		QMessageBox::critical(this->window(), "Failed to open file", "File \"" + filename + "\" could not be opened for reading.");
		return false;
	}

	QTextStream input(&file);

	this->setPlainText(input.readAll());

	return true;
}


bool MarkdownEditor::save(const QString& filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::critical(this->window(), "Failed to open file", "File \"" + filename + "\" could not be opened for writing.");
		return false;
	}

	QString contents = this->toPlainText();
	::toPlainText(contents);

	QTextStream output(&file);
	output.setCodec("UTF-8");
	output << contents;

	return true;
}


QString MarkdownEditor::getHtml()
{
	QString markdown = toPlainText();
	::toPlainText(markdown);
	return parseMarkdownPage(markdown);
}


void MarkdownEditor::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu* menu = createStandardContextMenu();
	//TODO custom menu items
	menu->exec(event->globalPos());
	delete menu;
}
