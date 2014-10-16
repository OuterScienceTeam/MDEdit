#include "defines.h"
#include "AboutDialog.h"

#include <QApplication>
#include <QDesktopWidget>
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

	textBrowser = new TextBrowser(this);
	textBrowser->setObjectName("AboutText");
	QPalette pal = palette();
	pal.setBrush(QPalette::Base, pal.background());
	textBrowser->setPalette(pal);
	layout->addWidget(textBrowser);

	QHBoxLayout* buttonLayout = new QHBoxLayout();
	layout->addLayout(buttonLayout);

	ok = new QPushButton("OK", this);
	connect(ok, SIGNAL(clicked()), this, SLOT(close()));
	ok->setFocus();

	buttonLayout->addStretch(1);
	buttonLayout->addWidget(ok, 0, Qt::AlignRight);

	QSize max = QApplication::desktop()->size();
	setFixedSize(ABOUT_DIALOG_WIDTH > max.width() ? max.width() : ABOUT_DIALOG_WIDTH,
				 ABOUT_DIALOG_HEIGHT > max.height() ? max.height() : ABOUT_DIALOG_HEIGHT);

	textBrowser->setSource(QUrl(ABOUT_DIALOG_URL));
}

AboutDialog::~AboutDialog()
{
	delete textBrowser;
	delete ok;
	delete icon;
}
