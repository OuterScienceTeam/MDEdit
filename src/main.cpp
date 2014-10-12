#include "defines.h"
#include "MDEdit.h"
#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setApplicationName(APPLICATION_NAME);
	a.setApplicationVersion(APPLICATION_VERSION);
	a.setOrganizationName(ORGANISATION_NAME);

	QCommandLineParser parser;
	parser.setApplicationDescription(APPLICATION_DESCRIPTION);
	parser.addHelpOption();
	parser.addVersionOption();

	parser.addPositionalArgument("filename(s)", "name(s) of file(s) to open");

	parser.process(a);

	MDEdit window;
	window.show();

	// open passed files
	QStringList args = parser.positionalArguments();
	foreach(const QString& arg, args)
	{
		window.newTab(arg);
	}

	return a.exec();
}
