#ifndef MARKDOWNPARSER_H
#define MARKDOWNPARSER_H

#include <QString>

/**
 * @brief parseMarkdownPage Parses Markdown page into HTML page
 * @param markdown The Markdown source to be turned into HTML page
 * @return parsed HTML page
 */
QString parseMarkdownPage(QString markdown);

#endif // MARKDOWNPARSER_H

