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

	MMIOT* document = mkd_string(data, dataArray.size(), 0);

	mkd_compile(document, 0);

	char* title = mkd_doc_title(document);
	char* author = mkd_doc_author(document);
	char* date = mkd_doc_date(document);
	char* css = 0;
	char* contents = 0;

	mkd_css(document, &css);
	mkd_document(document, &contents);

	QString page("<!Doctype html>\r\n"
				 "<html>\r\n"
				 "<head>\r\n"
				 "<meta http-equiv=\"Content-type\" content=\"text/html; charset=utf-8\"/>");
	if(date)
	{
		page.append("\t<meta http-equiv=\"Date\" content=\"");
		page.append(date);
		page.append("\"/>\r\n");
	}
	if(title)
	{
		page.append("\t<title>");
		page.append(title);
		page.append("</title>\r\n");
	}
	if(author)
	{
		page.append("\t<meta name=\"author\" content=\"");
		page.append(author);
		page.append("\" />\r\n");
	}
	if(css)
	{
		page.append(css).append("\r\n");
	}
	page.append("</head>\r\n");
	page.append("<body>\r\n");
	page.append(contents);
	page.append("\r\n</body>\r\n</html>");

	mkd_cleanup(document);

	return page;
}
