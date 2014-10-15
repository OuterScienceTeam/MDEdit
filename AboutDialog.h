#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QTextBrowser>
#include <QPushButton>
#include <QLabel>

#define WIDTH	500
#define HEIGHT	400

class AboutDialog : QDialog
{
	Q_OBJECT
private:
	QLabel* icon;
	QTextBrowser* textBrowser;
	QPushButton* ok;

public:
	AboutDialog(QWidget* parent = 0);
	~AboutDialog();

	static void showDialog();

private slots:
	void browserLinkClicked(const QUrl& url);
};

#endif // ABOUTDIALOG_H
