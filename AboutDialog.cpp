#include "defines.h"
#include "AboutDialog.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QDesktopServices>
#include <QVBoxLayout>
#include <QHBoxLayout>

AboutDialog::AboutDialog(QWidget *parent) :
	QDialog(parent)
{
	setWindowTitle("About "APPLICATION_NAME);

	delete this->layout();
	QVBoxLayout* layout = new QVBoxLayout(this);
	setLayout(layout);

	icon = new QLabel(this);
	icon->setAlignment(Qt::AlignCenter);
	icon->setPixmap(QPixmap(":/mdedit_icon_large.png"));
	layout->addWidget(icon);

	textBrowser = new QTextBrowser(this);
	textBrowser->setFrameStyle(QFrame::NoFrame);
	textBrowser->setFrameShadow(QFrame::Plain);
	textBrowser->setObjectName("AboutText");
	textBrowser->setReadOnly(true);
	textBrowser->setOpenLinks(false);
	QPalette pal = palette();
	pal.setBrush(QPalette::Base, pal.background());
	textBrowser->setPalette(pal);
	connect(textBrowser, SIGNAL(anchorClicked(QUrl)), this, SLOT(browserLinkClicked(QUrl)));
	layout->addWidget(textBrowser);

	QHBoxLayout* buttonLayout = new QHBoxLayout();
	layout->addLayout(buttonLayout);

	ok = new QPushButton("OK", this);
	connect(ok, SIGNAL(clicked()), this, SLOT(close()));
	ok->setFocus();

	buttonLayout->addStretch(1);
	buttonLayout->addWidget(ok, 0, Qt::AlignRight);

	QSize max = QApplication::desktop()->size();
	setFixedSize(WIDTH < max.width() ? WIDTH : max.width(),
				 HEIGHT < max.height() ? HEIGHT : max.height());
}

AboutDialog::~AboutDialog()
{
	delete textBrowser;
	delete ok;
	delete icon;
}

void AboutDialog::showDialog()
{
	AboutDialog* dialog = new AboutDialog();
	dialog->textBrowser->setHtml(APPLICATION_ABOUT_TEXT);
	dialog->exec();
	delete dialog;
}

void AboutDialog::browserLinkClicked(const QUrl &url)
{
	QDesktopServices::openUrl(url);
}
