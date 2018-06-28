SOURCES += \
    libPDF/pdfview.cpp \
    libPDF/pdfla.cpp

TEMPLATE = lib
TARGET = PDFLA

include(../../Project.pri)

INCLUDEPATH += $$INC_OUT_DIR
INCLUDEPATH += lib$$TARGET
QMAKE_LIBDIR += $$DESTDIR

LIBS += -lfpdfapi -lfpdfdoc -lfpdftext -lfxcodec -lfxcrt -lfxge

CONFIG += c++11
