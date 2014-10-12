#-------------------------------------------------
#
# QMake project for MDEdit
#
#-------------------------------------------------

QT += core gui widgets

TARGET = mdedit
LIBS += -lmarkdown

TEMPLATE = app

HEADERS  += \
	src/MDEdit.h \
	src/MarkdownEditor.h \
	src/EditorView.h \
	src/MarkdownParser.h \
	src/ThinSplitter.h \
	src/LineNumberWidget.h \
	src/defines.h

SOURCES += \
	src/main.cpp \
	src/MDEdit.cpp \
	src/MarkdownEditor.cpp \
	src/EditorView.cpp \
	src/MarkdownParser.cpp \
	src/ThinSplitter.cpp \
	src/LineNumberWidget.cpp
