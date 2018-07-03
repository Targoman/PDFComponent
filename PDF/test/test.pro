SOURCES += \
    render_pdf_test.cpp \
    layoutanalysis_test.cpp \
    main.cpp

TEMPLATE = app
TARGET = test_libPDF

include(../../Project.pri)

QT -= gui

INCLUDEPATH += $$INC_OUT_DIR
QMAKE_LIBDIR += $$LIB_OUT_DIR
DESTDIR = $$BIN_OUT_DIR

LIBS += -L$$LIB_OUT_DIR -lPDF -lfpdfapi -lfpdfdoc -lfpdftext -lfxcodec -lfdrm -lfxcrt -lfxge
#LIBS += -L$$LIB_OUT_DIR -lPDF

CONFIG += c++11
