SOURCES += \
    render_pdf_test.cpp \
    layoutanalysis_test.cpp \
    main.cpp

TEMPLATE = app
TARGET = test_libPDF

include(../../Project.pri)

INCLUDEPATH += $$INC_OUT_DIR
QMAKE_LIBDIR += $$LIB_OUT_DIR
DESTDIR = $$BIN_OUT_DIR

# LIBS += -lfpdfapi -lfpdfdoc -lfpdftext -lfxcodec -lfxcrt -lfxge

CONFIG += c++11
