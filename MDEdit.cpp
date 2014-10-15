#include "defines.h"
#include "MDEdit.h"

#include <QFileDialog>
#include <QVBoxLayout>
#include <QMimeData>
#include <QApplication>

#include "SaveFileQuestionDialog.h"
#include "AboutDialog.h"


MDEdit::MDEdit(QWidget *parent) :
	QMainWindow(parent),
	current(0)
{
	central = new QWidget(this);
	QVBoxLayout* layout = new QVBoxLayout(central);
	layout->setContentsMargins(0, 0, 0, 0);
	central->setLayout(layout);
	this->setCentralWidget(central);

	tabBar = new QTabBar(this);
	tabBar->setTabsClosable(true);
	tabBar->setMovable(true);
	tabBar->setExpanding(false);
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
	htmlPreview->setObjectName("HTMLPreview");
	splitter->addWidget(htmlPreview);

	splitter->setStretchFactor(0, 1);
	splitter->setStretchFactor(1, 0);


	setupToolbar();
	setupStatusbar();


	newTab();

	setAcceptDrops(true);
	setWindowIcon(QIcon(":/mdedit_icon.png"));
}


MDEdit::~MDEdit()
{
	delete menuBar;
	delete toolbar;
	delete statusbar;

	delete tabBar;
	delete htmlPreview;
	delete tabStack;
	delete splitter;
	delete central;
}


void MDEdit::closeEvent(QCloseEvent *event)
{
	if(!tabBar->count())
		return;

	SaveFileQuestionDialog dialog(this);

	bool cancel = false;
	for(int i = 0; i < tabBar->count() && !cancel; i++)
	{
		tabBar->setCurrentIndex(i);
		EditorView* tab = tabs.value(tabBar->tabData(i).toString());
		if(tab->isModified())
		{
			dialog.setFilename(tab->fullFilename());
			switch(dialog.exec())
			{
				case SaveFileQuestionDialog::Save:
					tab->save();
					if(tab->isModified())
					{
						cancel = true;
					}
					break;

				case SaveFileQuestionDialog::Cancel:
					cancel = true;
					break;

				case SaveFileQuestionDialog::Discard:
					break;
			}
		}
	}

	if(cancel)
	{
		event->setAccepted(false);
	}
}

void MDEdit::dragEnterEvent(QDragEnterEvent *event)
{
	if(event->mimeData()->hasUrls())
	{
		event->acceptProposedAction();
	}
}

void MDEdit::dropEvent(QDropEvent *event)
{
	foreach(const QUrl& fileUrl, event->mimeData()->urls())
	{
		newTab(fileUrl.toLocalFile());
	}

	event->acceptProposedAction();
}


void MDEdit::_tabCloseRequested(int index)
{
	QString key = tabBar->tabData(index).toString();

	EditorView* tab = tabs.value(key);

	if(tab->isModified())
	{
		SaveFileQuestionDialog dialog(this);
		dialog.setFilename(tab->fullFilename());
		switch(dialog.exec())
		{
			case SaveFileQuestionDialog::Save:
				tab->save();
				if(tab->isModified())
				{
					return;
				}
				break;

			case SaveFileQuestionDialog::Cancel:
				return;

			case SaveFileQuestionDialog::Discard:
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
	// disconnect
	if(current)
	{
		disconnect(this, SLOT(_tab_modificationChanged(bool)));
		disconnect(this, SLOT(_tab_filenameChanged()));
		disconnect(this, SLOT(_tab_changed()));
		disconnect(this, SLOT(_tab_cursorPositionChanged(int, int)));
		disconnect(this, SLOT(_tab_redoAvailable(bool)));
		disconnect(this, SLOT(_tab_undoAvailable(bool)));
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
	connect(tab, SIGNAL(modificationChanged(bool)), this, SLOT(_tab_modificationChanged(bool)));
	connect(tab, SIGNAL(filenameChanged()), this, SLOT(_tab_filenameChanged()));
	connect(tab, SIGNAL(changed()), this, SLOT(_tab_changed()));
	connect(tab, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(_tab_cursorPositionChanged(int, int)));
	connect(tab, SIGNAL(redoAvailable(bool)), this, SLOT(_tab_redoAvailable(bool)));
	connect(tab, SIGNAL(undoAvailable(bool)), this, SLOT(_tab_undoAvailable(bool)));
	current = tab;

	updateToolbar();
	updateLengthLabel(current->length());
	updatePositionLabel(current->cursorLine(), current->cursorCol());

	_tab_changed();

	updateWindowTitle();
}


void MDEdit::_tab_modificationChanged(bool changed)
{
	tabBar->setTabText(tabBar->currentIndex(), current->tabLabel());

	saveAction->setEnabled(changed);
}


void MDEdit::_tab_filenameChanged()
{
	tabs.remove(tabs.key(current));
	tabs.insert(current->fullFilename(), current);
	tabBar->setTabData(tabBar->currentIndex(), current->fullFilename());

	updateWindowTitle();
}


void MDEdit::_tab_changed()
{
	if(htmlPreview->isVisible())
	{
		htmlPreview->setHtml(current->getHtml());
	}

	updateLengthLabel(current->length());
}

void MDEdit::_tab_cursorPositionChanged(int line, int col)
{
	updatePositionLabel(line, col);
}

void MDEdit::_tab_redoAvailable(bool available)
{
	redoAction->setEnabled(available);
}

void MDEdit::_tab_undoAvailable(bool available)
{
	undoAction->setEnabled(available);
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

	// Automatically close the only empty tab
	//  if a new tab with a proper file is opened.
	bool oldTabToClose = tabs.count() == 1 && tabs.first()->isVirtual()
			&& !tabs.first()->isModified() && !newFile->isVirtual();

	tabBar->blockSignals(true);
	int index = tabBar->addTab(newFile->tabLabel());
	tabBar->setTabData(index, QVariant(newFile->fullFilename()));
	tabs.insert(newFile->fullFilename(), newFile);
	tabBar->setCurrentIndex(index);
	tabStack->addWidget(newFile);
	tabBar->blockSignals(false);
	emit tabBar->currentChanged(index);

	if(oldTabToClose)
	{
		_tabCloseRequested(0);
	}
}


void MDEdit::openFile()
{
	QStringList filenames = QFileDialog::getOpenFileNames(this, "Open", QString(), "Markdown (*.md *.markdown);;Any file (*.*)");
	foreach(const QString& filename, filenames)
	{
		newTab(filename);
	}
}


void MDEdit::save()
{
	if(current)
		current->save();
}


void MDEdit::saveAs()
{
	if(current)
		current->saveAs();
}


void MDEdit::undo()
{
	if(current)
		current->undo();
}


void MDEdit::redo()
{
	if(current)
		current->redo();
}


void MDEdit::updateWindowTitle()
{
	setWindowTitle(current->filename() + " - " APPLICATION_NAME);
}


void MDEdit::setupToolbar()
{
	menuBar = new QMenuBar(this);
	menuBar->setObjectName("MenuBar");
	this->setMenuBar(menuBar);

	toolbar = new QToolBar(this);
	toolbar->setMovable(false);
	this->addToolBar(Qt::TopToolBarArea, toolbar);

	QMenu* fileMenu = menuBar->addMenu("File");
	QAction* newAction = fileMenu->addAction("New", this, SLOT(newTab()));
	openAction = fileMenu->addAction("Open", this, SLOT(openFile()));

	saveAction = fileMenu->addAction("Save", this, SLOT(save()));
	saveAction->setDisabled(true);
	saveAction->setShortcut(QKeySequence(QKeySequence::Save));

	saveAsAction = fileMenu->addAction("Save As", this, SLOT(saveAs()));
	saveAsAction->setShortcut(QKeySequence(QKeySequence::SaveAs));

	fileMenu->addSeparator();

	QAction* exitAction = fileMenu->addAction("Exit", QApplication::instance(), SLOT(quit()));
	exitAction->setShortcut(QKeySequence(QKeySequence::Quit));

	QMenu* editMenu = menuBar->addMenu("Edit");
	undoAction = editMenu->addAction("Undo", this, SLOT(undo()));
	redoAction = editMenu->addAction("Redo", this, SLOT(redo()));


	QMenu* viewMenu = menuBar->addMenu("View");
	QAction* previewAction = viewMenu->addAction("Show HTML preview");
	previewAction->setCheckable(true);
	previewAction->setChecked(true);
	connect(previewAction, SIGNAL(toggled(bool)), htmlPreview, SLOT(setVisible(bool)));


	QMenu* helpMenu = menuBar->addMenu("Help");
	helpMenu->addAction("About", this, SLOT(showAbout()));
	helpMenu->addAction("About Qt", this, SLOT(showAboutQt()));



	// add actions to the toolbar
	toolbar->addAction(newAction);
	toolbar->addAction(openAction);
	toolbar->addAction(saveAction);
	toolbar->addAction(saveAsAction);
	toolbar->addSeparator();
	toolbar->addAction(undoAction);
	toolbar->addAction(redoAction);
	toolbar->addSeparator();
	toolbar->addAction(previewAction);
}

void MDEdit::updateToolbar()
{
	saveAction->setEnabled(current->isModified());
	_tab_redoAvailable(current->isRedoAvailable());
	_tab_undoAvailable(current->isUndoAvailable());
}


void MDEdit::setupStatusbar()
{
	statusbar = new QStatusBar(this);
	statusbar->setObjectName("StatusBar");
	this->setStatusBar(statusbar);
	lengthLabel = new QLabel(this);
	statusbar->addWidget(lengthLabel, 1);
	positionLabel = new QLabel(this);
	statusbar->addWidget(positionLabel, 1);
	updateLengthLabel(0);
	updatePositionLabel(0, 0);
}

void MDEdit::updateLengthLabel(int length)
{
	lengthLabel->setText(QString("Length: %1").arg(length));
}

void MDEdit::updatePositionLabel(int line, int col)
{
	positionLabel->setText(QString("Line: %1, Col: %2").arg(line).arg(col));
}


void MDEdit::showAbout()
{
	AboutDialog::showDialog();
}

void MDEdit::showAboutQt()
{
	QApplication::aboutQt();
}
