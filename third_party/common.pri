TEMPLATE = lib
CONFIG += create_prl
CONFIG += staticlib
INCLUDEPATH += $$PWD/pdfiumjs

QMAKE_CXXFLAGS += -w
QMAKE_CFLAGS += -w

include(../Project.pri)