#include "SaveFileQuestionDialog.h"

SaveFileQuestionDialog::SaveFileQuestionDialog(QWidget* parent) :
	QMessageBox(parent),
	textTemplate("File \"%1\" was modified.\nDo you wish to save it?")
{
	setModal(false);
	setIcon(QMessageBox::Question);
	addButton(QMessageBox::Save);
	addButton(QMessageBox::Cancel);
	addButton(QMessageBox::Discard);
	setDefaultButton(QMessageBox::Cancel);
}

void SaveFileQuestionDialog::setFilename(const QString& filename)
{
	setText(textTemplate.arg(filename));
}
