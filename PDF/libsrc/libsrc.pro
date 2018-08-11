SOURCES += \
    libPDF/clsPDF.cpp \
    libPDF/LayoutAnalyser.cpp \
    libPDF/clsParagraph.cpp

TEMPLATE = lib
TARGET = PDF

include(../../Project.pri)

QT -= gui
QT -= core

INCLUDEPATH += $$INC_OUT_DIR
INCLUDEPATH += lib$$TARGET
QMAKE_LIBDIR += $$LIB_OUT_DIR

LIBS += -L$$LIB_OUT_DIR -lfdrm -lfpdfapi -lfpdfdoc -lfpdftext -lfxcodec -lfxcrt -lfxge

CONFIG += c++11
CONFIG += staticlib

HEADERS += \
    libPDF/clsPDF.h \
    libPDF/LayoutAnalyser.h \
    libPDF/clsParagraph.h

QMAKE_POST_BUILD += ./postbuild.sh $$INC_OUT_DIR/ "libPDF/clsPDF.h" "libPDF/clsParagraph.h"
