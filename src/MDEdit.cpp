#include "MDEdit.h"

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>


MDEdit::MDEdit(QWidget *parent) :
	QMainWindow(parent)
{
	current = 0;

	setupToolbar();

	tabWidget = new QTabWidget(this);
	this->setCentralWidget(tabWidget);
	tabWidget->setTabsClosable(true);
	tabWidget->setMovable(true);

	connect(tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(_tabCloseRequested(int)));
	connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(_currentTabChanged(int)));

	EditorView* tab1 = new EditorView(QString());
	tabWidget->addTab(tab1, tab1->filename());

	EditorView* tab2 = new EditorView(QString());
	tabWidget->addTab(tab2, tab2->filename());

	statusbar = new QStatusBar(this);
	this->setStatusBar(statusbar);
}


MDEdit::~MDEdit()
{
	delete toolbar;
	delete tabWidget;
	delete statusbar;
}


void MDEdit::_tabCloseRequested(int index)
{
	EditorView* tab = reinterpret_cast<EditorView*>(tabWidget->widget(index));
	qDebug() << index << tab->filename();

	if(tab->isChanged())
	{
		QMessageBox dialog(this);
		dialog.setModal(false);
		dialog.setIcon(QMessageBox::Question);
		dialog.addButton(QMessageBox::Save);
		dialog.addButton(QMessageBox::Cancel);
		dialog.addButton(QMessageBox::Discard);
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

	tabWidget->removeTab(index);
}


void MDEdit::_currentTabChanged(int index)
{
	qDebug() << index << "changed";

	// disconnect
	if(current)
	{
		disconnect(this, SLOT(_tab_changed(bool)));
		disconnect(current, SLOT(save()));
	}

	if(index == -1)
	{
		current = 0;
		return;
	}

	EditorView* tab = reinterpret_cast<EditorView*>(tabWidget->widget(index));

	// reconnect
	connect(tab, SIGNAL(changed(bool)), this, SLOT(_tab_changed(bool)));
	connect(saveAction, SIGNAL(triggered()), tab, SLOT(save()));
	current = tab;
}


void MDEdit::_tab_changed(bool changed)
{
	EditorView* tab = reinterpret_cast<EditorView*>(QObject::sender());
	//update label
	tabWidget->setTabText(tabWidget->currentIndex(), tab->tabLabel());

	saveAction->setEnabled(changed);
}


void MDEdit::newTab()
{
	EditorView* tab = new EditorView(QString());
	tabWidget->addTab(tab, tab->filename());
}


void MDEdit::loadFile()
{
	QStringList filenames = QFileDialog::getOpenFileNames(this, "Open", QString(), "Markdown (*.md, *.markdown);;Any file (*.*)");
	foreach(const QString& filename, filenames)
	{
		EditorView* newFile = new EditorView(filename);
		tabWidget->addTab(newFile, newFile->tabLabel());
	}
}


void MDEdit::setupToolbar()
{
	toolbar = new QToolBar(this);
	toolbar->setMovable(false);

	toolbar->addAction("New", this, SLOT(newTab()));

	loadAction = toolbar->addAction("Load", this, SLOT(loadFile()));

	saveAction = toolbar->addAction("Save");
	saveAction->setDisabled(true);
	saveAction->setShortcut(QKeySequence(QKeySequence::Save));

	saveAsAction = toolbar->addAction("Save As");
	saveAsAction->setShortcut(QKeySequence(QKeySequence::SaveAs));
	//connect(saveAsAction, SIGNAL(triggered()), this, SLOT(openFileSaveDialog()));

	toolbar->addSeparator();

	this->addToolBar(Qt::TopToolBarArea, toolbar);
}
