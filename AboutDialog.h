#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>

#include "TextBrowser.h"

#define ABOUT_DIALOG_WIDTH	500
#define ABOUT_DIALOG_HEIGHT	400

class AboutDialog : public QDialog
{
	Q_OBJECT
private:
	QLabel* icon;
	TextBrowser* textBrowser;
	QPushButton* ok;

public:
	AboutDialog(QWidget* parent = 0);
	~AboutDialog();
};

#endif // ABOUTDIALOG_H
