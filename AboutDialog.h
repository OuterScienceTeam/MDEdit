#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>

#include "TextBrowser.h"

#define WIDTH	500
#define HEIGHT	400

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
