#include "defines.h"
#include "MDEdit.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QFile>
#include <QTextStream>
#include <QDebug>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setApplicationName(APPLICATION_NAME);
	app.setApplicationVersion(APPLICATION_VERSION);
	app.setOrganizationName(ORGANISATION_NAME);

	QCommandLineParser parser;
	parser.setApplicationDescription(APPLICATION_DESCRIPTION);
	parser.addHelpOption();
	parser.addVersionOption();
	parser.addPositionalArgument("filename(s)", "name(s) of file(s) to open");

	parser.process(app);


	// load stylesheet
	QFile* stylesheet = new QFile(":/style.css");
	if(stylesheet->open(QFile::ReadOnly))
	{
		QTextStream stream(stylesheet);
		app.setStyleSheet(app.styleSheet() + stream.readAll());
		stylesheet->close();
	}
	delete stylesheet;


	MDEdit window;
	window.show();

	// open passed files
	QStringList args = parser.positionalArguments();
	foreach(const QString& arg, args)
	{
		window.newTab(arg);
	}

	return app.exec();
}
