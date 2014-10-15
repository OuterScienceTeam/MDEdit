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
	defines.h \
    SaveFileQuestionDialog.h \
    AboutDialog.h

SOURCES += \
	main.cpp \
	MDEdit.cpp \
	MarkdownEditor.cpp \
	EditorView.cpp \
	MarkdownParser.cpp \
	ThinSplitter.cpp \
	LineNumberWidget.cpp \
    SaveFileQuestionDialog.cpp \
    AboutDialog.cpp

RESOURCES += \
    data.qrc
