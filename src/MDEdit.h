#ifndef MDEDIT_H
#define MDEDIT_H

#include <QMainWindow>
#include <QToolBar>
#include <QTabWidget>
#include <QStatusBar>
#include <QList>
#include <QAction>

#include "EditorView.h"


class MDEdit : public QMainWindow
{
	Q_OBJECT
private:
	QToolBar* toolbar;
	QAction* saveAction;
	QAction* loadAction;
	QAction* saveAsAction;

	QTabWidget* tabWidget;

	QStatusBar* statusbar;

	EditorView* current;

	void setupToolbar();

public:
	explicit MDEdit(QWidget *parent = 0);
	~MDEdit();

private slots:
	void loadFile();
	void _tabCloseRequested(int index);
	void _currentTabChanged(int index);
	void _tab_changed(bool);
};

#endif // MDEDIT_H
