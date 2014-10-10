#include "MDEdit.h"
#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setApplicationName("MDEdit");
	a.setApplicationVersion("0.2-alpha");
	a.setOrganizationName("OuterScience");

	QCommandLineParser parser;
	parser.setApplicationDescription("MDEdit - a Markdown Editor");
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
