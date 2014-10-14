#include "defines.h"
#include "MDEdit.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QMimeData>


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
	htmlPreview->setStyleSheet("QTextBrowser { background: #F5F5F5; }");
	splitter->addWidget(htmlPreview);

	splitter->setStretchFactor(0, 1);
	splitter->setStretchFactor(1, 0);


	setupToolbar();
	setupStatusbar();


	newTab();

	setAcceptDrops(true);
	setWindowIcon(QIcon(":/MDEdit.png"));
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


void MDEdit::closeEvent(QCloseEvent *event)
{
	bool cancel = false;
	for(int i = 0; i < tabBar->count(); i++)
	{
		tabBar->setCurrentIndex(i);
		EditorView* tab = tabs.value(tabBar->tabData(i).toString());
		if(tab->isModified())
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
					if(tab->isModified())
					{
						cancel = true;
					}
					break;

				case QMessageBox::Cancel:
					cancel = true;
					break;

				case QMessageBox::Discard:
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
				if(tab->isModified())
				{
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
	// disconnect
	if(current)
	{
		disconnect(this, SLOT(_tab_modificationChanged(bool)));
		disconnect(this, SLOT(_tab_filenameChanged()));
		disconnect(this, SLOT(_tab_changed()));
		disconnect(this, SLOT(_tab_cursorPositionChanged(int, int)));
		disconnect(this, SLOT(_tab_redoAvailable(bool)));
		disconnect(this, SLOT(_tab_undoAvailable(bool)));
		disconnect(current, SLOT(save()));
		disconnect(current, SLOT(saveAs()));
		disconnect(current, SLOT(redo()));
		disconnect(current, SLOT(undo()));
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
	connect(saveAction, SIGNAL(triggered()), tab, SLOT(save()));
	connect(saveAsAction, SIGNAL(triggered()), tab, SLOT(saveAs()));
	connect(redoAction, SIGNAL(triggered()), tab, SLOT(redo()));
	connect(undoAction, SIGNAL(triggered()), tab, SLOT(undo()));
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


void MDEdit::exportHtml()
{
	current->exportHtml();
}


void MDEdit::updateWindowTitle()
{
	setWindowTitle(current->filename() + " - " APPLICATION_NAME);
}


void MDEdit::setupToolbar()
{
	toolbar = new QToolBar(this);
	toolbar->setMovable(false);

	toolbar->addAction("New", this, SLOT(newTab()));

	openAction = toolbar->addAction("Open", this, SLOT(openFile()));

	saveAction = toolbar->addAction("Save");
	saveAction->setDisabled(true);
	saveAction->setShortcut(QKeySequence(QKeySequence::Save));

	saveAsAction = toolbar->addAction("Save As");
	saveAsAction->setShortcut(QKeySequence(QKeySequence::SaveAs));

	toolbar->addSeparator();

	undoAction = toolbar->addAction("Undo");
	redoAction = toolbar->addAction("Redo");

	toolbar->addSeparator();

	QAction* previewAction = toolbar->addAction("Preview");
	previewAction->setCheckable(true);
	previewAction->setChecked(true);
	connect(previewAction, SIGNAL(toggled(bool)), htmlPreview, SLOT(setVisible(bool)));

	toolbar->addSeparator();

	toolbar->addAction("Export HTML", this, SLOT(exportHtml()));

	this->addToolBar(Qt::TopToolBarArea, toolbar);
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
	this->setStatusBar(statusbar);
	lengthLabel = new QLabel(this);
	lengthLabel->setFont(QFont("DejaVu Serif", 8));
	statusbar->addWidget(lengthLabel, 1);
	positionLabel = new QLabel(this);
	positionLabel->setFont(QFont("DejaVu Serif", 8));
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
