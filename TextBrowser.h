#ifndef TEXTBROWSER_H
#define TEXTBROWSER_H

#include <QTextBrowser>

class TextBrowser : public QTextBrowser
{
	Q_OBJECT
public:
	TextBrowser(QWidget* parent = 0);

private slots:
	void slotAnchorClicked(const QUrl& url);
};

#endif // TEXTBROWSER_H
