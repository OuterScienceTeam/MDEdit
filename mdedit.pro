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
	MDEdit.h \
	MarkdownEditor.h \
	EditorView.h \
	MarkdownParser.h \
	ThinSplitter.h \
	LineNumberWidget.h \
	defines.h

SOURCES += \
	main.cpp \
	MDEdit.cpp \
	MarkdownEditor.cpp \
	EditorView.cpp \
	MarkdownParser.cpp \
	ThinSplitter.cpp \
	LineNumberWidget.cpp

RESOURCES += \
    data.qrc
