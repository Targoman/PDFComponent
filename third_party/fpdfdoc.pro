SOURCES += \
            pdfiumjs/core/src/fpdfdoc/doc_action.cpp \
            pdfiumjs/core/src/fpdfdoc/doc_annot.cpp \
            pdfiumjs/core/src/fpdfdoc/doc_ap.cpp \
            pdfiumjs/core/src/fpdfdoc/doc_basic.cpp \
            pdfiumjs/core/src/fpdfdoc/doc_bookmark.cpp \
            pdfiumjs/core/src/fpdfdoc/doc_form.cpp \
            pdfiumjs/core/src/fpdfdoc/doc_formcontrol.cpp \
            pdfiumjs/core/src/fpdfdoc/doc_formfield.cpp \
            pdfiumjs/core/src/fpdfdoc/doc_link.cpp \
            pdfiumjs/core/src/fpdfdoc/doc_metadata.cpp \
            pdfiumjs/core/src/fpdfdoc/doc_ocg.cpp \
            pdfiumjs/core/src/fpdfdoc/doc_tagged.cpp \
            pdfiumjs/core/src/fpdfdoc/doc_utils.cpp \
            pdfiumjs/core/src/fpdfdoc/doc_viewerPreferences.cpp \
            pdfiumjs/core/src/fpdfdoc/doc_vt.cpp \
            pdfiumjs/core/src/fpdfdoc/doc_vtmodule.cpp

HEADERS += \
            pdfiumjs/core/include/fpdfdoc/fpdf_ap.h \
            pdfiumjs/core/include/fpdfdoc/fpdf_doc.h \
            pdfiumjs/core/include/fpdfdoc/fpdf_tagged.h \
            pdfiumjs/core/include/fpdfdoc/fpdf_vt.h \
            pdfiumjs/core/src/fpdfdoc/pdf_vt.h \
            pdfiumjs/core/src/fpdfdoc/tagged_int.h

ProjectName=fpdfdoc
include(./common.pri)