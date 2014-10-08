#include "MDEdit.h"

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QVBoxLayout>


MDEdit::MDEdit(QWidget *parent) :
	QMainWindow(parent),
	current(0)
{
	statusbar = new QStatusBar(this);
	this->setStatusBar(statusbar);

	central = new QWidget(this);
	QVBoxLayout* layout = new QVBoxLayout(central);
	layout->setContentsMargins(0, 0, 0, 0);
	central->setLayout(layout);
	this->setCentralWidget(central);

	tabBar = new QTabBar(this);
	tabBar->setTabsClosable(true);
	tabBar->setMovable(true);
	layout->addWidget(tabBar);

	connect(tabBar, SIGNAL(tabCloseRequested(int)), this, SLOT(_tabCloseRequested(int)));
	connect(tabBar, SIGNAL(currentChanged(int)), this, SLOT(_currentTabChanged(int)));


	splitter = new ThinSplitter(this);
	splitter->setChildrenCollapsible(false);
	layout->addWidget(splitter);

	tabStack = new QStackedWidget(this);
	splitter->addWidget(tabStack);

	htmlPreview = new QTextBrowser(this);
	htmlPreview->setFrameStyle(QFrame::NoFrame);
	htmlPreview->setFrameShadow(QFrame::Plain);
	htmlPreview->setStyleSheet("background: #F5F5F5;");
	splitter->addWidget(htmlPreview);

	splitter->setStretchFactor(0, 1);
	splitter->setStretchFactor(1, 0);


	setupToolbar();


	newTab();
}


MDEdit::~MDEdit()
{
	delete toolbar;
	delete statusbar;

	delete tabBar;
	delete htmlPreview;
	delete tabStack;
	delete splitter;
	delete central;
}


void MDEdit::_tabCloseRequested(int index)
{
	QString key = tabBar->tabData(index).toString();

	EditorView* tab = tabs.value(key);

	qDebug() << index << tab->filename();

	if(tab->isChanged())
	{
		QMessageBox dialog(this);
		dialog.setModal(false);
		dialog.setIcon(QMessageBox::Question);
		dialog.addButton(QMessageBox::Save);
		dialog.addButton(QMessageBox::Cancel);
		dialog.addButton(QMessageBox::Discard);
		dialog.setDefaultButton(QMessageBox::Cancel);
		dialog.setText("File \"" + tab->filename() + "\" was modified.\nDo you wish to save it?");
		switch(dialog.exec())
		{
			case QMessageBox::Save:
				tab->save();
				if(tab->isChanged())
				{
					qDebug() << index << "save failed";
					return;
				}
				break;

			case QMessageBox::Cancel:
				return;

			case QMessageBox::Discard:
				break;
		}
	}

	tabs.remove(key);
	tabStack->removeWidget(tab);
	tabBar->removeTab(index);
	delete tab;
	tabBar->repaint();
}


void MDEdit::_currentTabChanged(int index)
{
	qDebug() << index << "changed";

	// disconnect
	if(current)
	{
		disconnect(this, SLOT(_tab_changed(bool)));
		disconnect(this, SLOT(_tab_filenameChanged()));
		disconnect(this, SLOT(_tab_changed()));
		disconnect(current, SLOT(save()));
		disconnect(current, SLOT(saveAs()));
	}

	if(index == -1)
	{
		current = 0;
		newTab();
		return;
	}

	QString key = tabBar->tabData(index).toString();
	EditorView* tab = tabs.value(key);

	tabStack->setCurrentWidget(tab);

	// reconnect
	connect(tab, SIGNAL(changed(bool)), this, SLOT(_tab_changed(bool)));
	connect(tab, SIGNAL(filenameChanged()), this, SLOT(_tab_filenameChanged()));
	connect(tab, SIGNAL(changed()), this, SLOT(_tab_changed()));
	connect(saveAction, SIGNAL(triggered()), tab, SLOT(save()));
	connect(saveAsAction, SIGNAL(triggered()), tab, SLOT(saveAs()));
	current = tab;

	_tab_changed();
}


void MDEdit::_tab_changed(bool /*changed*/)
{
	updateUI();
}


void MDEdit::_tab_filenameChanged()
{
	tabs.remove(tabs.key(current));
	tabs.insert(current->fullFilename(), current);
	tabBar->setTabData(tabBar->currentIndex(), current->fullFilename());

	updateUI();
}


void MDEdit::_tab_changed()
{
	if(htmlPreview->isVisible())
	{
		htmlPreview->setHtml(current->getHtml());
	}
}


void MDEdit::newTab(const QString& filename)
{
	QFileInfo file(filename);
	QString key = file.absoluteFilePath();
	if(tabs.contains(key))
	{
		// focus the tab
		for(int i = 0; i < tabBar->count(); i++)
		{
			if(tabBar->tabData(i).toString() == key)
			{
				tabBar->setCurrentIndex(i);
				break;
			}
		}
		return;
	}

	EditorView* newFile = new EditorView(filename);
	tabBar->blockSignals(true);
	int index = tabBar->addTab(newFile->tabLabel());
	tabBar->setTabData(index, QVariant(newFile->fullFilename()));
	tabs.insert(newFile->fullFilename(), newFile);
	tabBar->setCurrentIndex(index);
	tabStack->addWidget(newFile);
	tabBar->blockSignals(false);
	emit tabBar->currentChanged(index);
}


void MDEdit::openFile()
{
	QStringList filenames = QFileDialog::getOpenFileNames(this, "Open", QString(), "Markdown (*.md *.markdown);;Any file (*.*)");
	foreach(const QString& filename, filenames)
	{
		newTab(filename);
	}
}


void MDEdit::updateUI()
{
	tabBar->setTabText(tabBar->currentIndex(), current->tabLabel());

	saveAction->setEnabled(current->isChanged());
}


void MDEdit::setupToolbar()
{
	toolbar = new QToolBar(this);
	toolbar->setMovable(false);

	toolbar->addAction("New", this, SLOT(newTab()));

	loadAction = toolbar->addAction("Load", this, SLOT(openFile()));

	saveAction = toolbar->addAction("Save");
	saveAction->setDisabled(true);
	saveAction->setShortcut(QKeySequence(QKeySequence::Save));

	saveAsAction = toolbar->addAction("Save As");
	saveAsAction->setShortcut(QKeySequence(QKeySequence::SaveAs));

	toolbar->addSeparator();

	QAction* previewAction = toolbar->addAction("Preview");
	previewAction->setCheckable(true);
	previewAction->setChecked(true);
	connect(previewAction, SIGNAL(toggled(bool)), htmlPreview, SLOT(setVisible(bool)));


	this->addToolBar(Qt::TopToolBarArea, toolbar);
}
