TEMPLATE = lib
CONFIG += create_prl
CONFIG += staticlib
INCLUDEPATH += $$PWD/pdfiumjs

QMAKE_CXXFLAGS += -w
QMAKE_CFLAGS += -w

include(../Project.pri)

QMAKE_POST_BUILD += mkdir -p $$INC_OUT_DIR/pdfiumjs/ && cp $$HEADERS $$INC_OUT_DIR/pdfiumjs/