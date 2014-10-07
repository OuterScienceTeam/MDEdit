#include "MDEdit.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setApplicationName("MDEdit");
	a.setOrganizationName("OuterScience");

	MDEdit window;
	window.show();

	return a.exec();
}
