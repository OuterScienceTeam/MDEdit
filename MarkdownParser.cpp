#include "MarkdownParser.h"

#include <cstring>

extern "C"
{
	#include <mkdio.h>
}
#include <QDebug>

QString parseMarkdownPage(QString markdown)
{
	QByteArray dataArray = markdown.toUtf8();
	char* data = dataArray.data();

	MMIOT* document = gfm_string(data, dataArray.size(), 0);

	mkd_compile(document, 0);

	char* title = mkd_doc_title(document);
	char* author = mkd_doc_author(document);
	char* date = mkd_doc_date(document);
	char* css = 0;
	char* contents = 0;

	mkd_css(document, &css);
	mkd_document(document, &contents);

	QString page("<!Doctype html>\n"
				 "<html>\n"
				 "<head>\n"
				 "\t<meta http-equiv=\"Content-type\" content=\"text/html; charset=utf-8\"/>\n");
	if(title)
	{
		page.append("\t<title>");
		page.append(title);
		page.append("</title>\n");
	}
	if(author)
	{
		page.append("\t<meta name=\"author\" content=\"");
		page.append(author);
		page.append("\" />\n");
	}
	if(date)
	{
		page.append("\t<meta http-equiv=\"Date\" content=\"");
		page.append(date);
		page.append("\"/>\n");
	}
	if(css)
	{
		page.append(css).append("\n");
	}
	page.append("</head>\n");
	page.append("<body>\n");
	page.append(contents);
	page.append("\n</body>\n</html>");

	mkd_cleanup(document);

	return page;
}
