#ifndef MDEDIT_H
#define MDEDIT_H

#include <QMainWindow>
#include <QToolBar>
#include <QStatusBar>
#include <QTabBar>
#include <QStackedWidget>
#include <QMap>
#include <QAction>
#include <QTextBrowser>
#include <QLabel>

#include "EditorView.h"
#include "ThinSplitter.h"

class MDEdit : public QMainWindow
{
	Q_OBJECT
private:
	QToolBar* toolbar;
	QAction* saveAction;
	QAction* loadAction;
	QAction* saveAsAction;

	QStatusBar* statusbar;
	QLabel* lengthLabel;
	QLabel* positionLabel;

	QWidget* central;
	QMap<QString, EditorView*> tabs;
	QTabBar* tabBar;
	QStackedWidget* tabStack;
	EditorView* current;

	ThinSplitter* splitter;
	QTextBrowser* htmlPreview;

	void setupToolbar();
	void setupStatusbar();
	void updateUI();
	inline void updateLengthLabel(int length);
	inline void updatePositionLabel(int line, int col);

public:
	MDEdit(QWidget *parent = 0);
	~MDEdit();

private slots:
	void newTab(const QString& filename = QString());
	void openFile();
	void exportHtml();

	void _tabCloseRequested(int index);
	void _currentTabChanged(int index);
	void _tab_changed(bool);
	void _tab_filenameChanged();
	void _tab_changed();
	void _tab_cursorPositionChanged(int line, int col);
};

#endif // MDEDIT_H
