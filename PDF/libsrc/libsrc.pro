SOURCES += \
    libPDF/clsPDF.cpp

TEMPLATE = lib
TARGET = PDF

include(../../Project.pri)

QT -= gui

INCLUDEPATH += $$INC_OUT_DIR
INCLUDEPATH += lib$$TARGET
QMAKE_LIBDIR += $$LIB_OUT_DIR

LIBS += -L$$LIB_OUT_DIR -lfdrm -lfpdfapi -lfpdfdoc -lfpdftext -lfxcodec -lfxcrt -lfxge

CONFIG += c++11

HEADERS += \
    libPDF/clsPDF.h
