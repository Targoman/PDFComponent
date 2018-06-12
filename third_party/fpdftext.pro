SOURCES += \
        pdfiumjs/core/src/fpdftext/fpdf_text.cpp \
        pdfiumjs/core/src/fpdftext/fpdf_text_int.cpp \
        pdfiumjs/core/src/fpdftext/fpdf_text_search.cpp \
        pdfiumjs/core/src/fpdftext/unicodenormalization.cpp \
        pdfiumjs/core/src/fpdftext/unicodenormalizationdata.cpp

HEADERS += \
        pdfiumjs/core/include/fpdftext/fpdf_text.h \
        pdfiumjs/core/src/fpdftext/text_int.h \
        pdfiumjs/core/src/fpdftext/txtproc.h

ProjectName=fpdftext
include(./common.pri)