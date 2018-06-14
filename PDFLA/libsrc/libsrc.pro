SOURCES += \
    libPDFLA/pdfla.cpp


TEMPLATE = lib
TARGET = PDFLA

include(../../Project.pri)

INCLUDEPATH += $$INC_OUT_DIR
INCLUDEPATH += lib$$TARGET
QMAKE_LIBDIR += $$DESTDIR

LIBS += -llibfpdfapi.a -llibfpdfdoc.a -llibfpdftext.a -llibfxcodec.a -llibfxcrt.a -llibfxge.a

CONFIG += c++11