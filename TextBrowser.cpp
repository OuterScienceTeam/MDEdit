#include "TextBrowser.h"
#include <QDesktopServices>
#include <QMessageBox>

TextBrowser::TextBrowser(QWidget* parent) :
	QTextBrowser(parent)
{
	setFrameStyle(QFrame::NoFrame);
	setFrameShadow(QFrame::Plain);
	setReadOnly(true);
	setOpenLinks(false);
	connect(this, SIGNAL(anchorClicked(QUrl)), this, SLOT(slotAnchorClicked(QUrl)));
}

void TextBrowser::slotAnchorClicked(const QUrl &url)
{
	if(!QDesktopServices::openUrl(url))
	{
		QMessageBox::warning(window(), QString(), "Failed to open link.");
	}
}
