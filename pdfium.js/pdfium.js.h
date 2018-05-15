#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <utility>

#include "../fpdfsdk/include/fpdf_dataavail.h"
#include "../fpdfsdk/include/fpdf_ext.h"
#include "../fpdfsdk/include/fpdfformfill.h"
#include "../fpdfsdk/include/fpdftext.h"
#include "../fpdfsdk/include/fpdfview.h"

struct __GLOBAL__ {
    UNSUPPORT_INFO unsuppored_info;
};

struct PDFiumJS_Doc {
    PDFiumJS_Doc(void *file_id, size_t len);
    ~PDFiumJS_Doc();
    static inline int Form_Alert(IPDF_JSPLATFORM*, FPDF_WIDESTRING, FPDF_WIDESTRING, int, int) {  return 0; }
    static inline bool Is_Data_Avail(FX_FILEAVAIL *pThis, size_t offset, size_t size) { return true; }
    static void Add_Segment(FX_DOWNLOADHINTS *pThis, size_t offset, size_t size) { /* Do nothing */ }
    IPDF_JSPLATFORM platform_callbacks;
    FPDF_FORMFILLINFO form_callbacks;
    FPDF_FILEACCESS file_access;
    FX_FILEAVAIL file_avail;
    FX_DOWNLOADHINTS hints;
    FPDF_AVAIL pdf_avail;
    FPDF_DOCUMENT doc;
    FPDF_FORMHANDLE form;
    int page_count;
};

struct PDFiumJS_Page {
    PDFiumJS_Page(FPDF_PAGE page, PDFiumJS_Doc *doc) 
        : page(page), doc(doc) 
    { }
    FPDF_PAGE page;
    PDFiumJS_Doc *doc;
};

extern __GLOBAL__ global;