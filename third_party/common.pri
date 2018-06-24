TEMPLATE = lib
CONFIG += create_prl
CONFIG += staticlib
QT -= gui
QT -= core
INCLUDEPATH += $$PWD/pdfiumjs

QMAKE_CXXFLAGS += -w
QMAKE_CFLAGS += -w

include(../Project.pri)

QMAKE_POST_BUILD += ./postbuild.sh $$INC_OUT_DIR/pdfiumjs/ $$HEADERS
