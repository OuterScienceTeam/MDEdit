#ifndef SAVEFILEQUESTIONDIALOG_H
#define SAVEFILEQUESTIONDIALOG_H

#include <QMessageBox>

class SaveFileQuestionDialog : public QMessageBox
{
	Q_OBJECT
private:
	QString textTemplate;

public:
	SaveFileQuestionDialog(QWidget* parent = 0);
	void setFilename(const QString& filename);
};

#endif // SAVEFILEQUESTIONDIALOG_H
