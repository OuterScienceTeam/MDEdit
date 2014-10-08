#ifndef MDEDIT_H
#define MDEDIT_H

#include <QMainWindow>
#include <QToolBar>
#include <QStatusBar>
#include <QTabBar>
#include <QStackedWidget>
#include <QMap>
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

	QStatusBar* statusbar;

	QWidget* central;
	QMap<QString, EditorView*> tabs;
	QTabBar* tabBar;
	QStackedWidget* tabStack;
	EditorView* current;

	void setupToolbar();

public:
	explicit MDEdit(QWidget *parent = 0);
	~MDEdit();

private slots:
	void newTab(const QString& filename = QString());
	void loadFile();

	void _tabCloseRequested(int index);
	void _currentTabChanged(int index);
	void _tab_changed(bool);
	void _tab_filenameChanged();
};

#endif // MDEDIT_H
