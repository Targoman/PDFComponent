// Copyright (c) 2018 Targoman Intelligent Processing Co.

#include "./pdfium.js.h"

struct PDFiumJS_LayoutAnalysisResult {
    int blockCount;
};

extern "C"
PDFiumJS_LayoutAnalysisResult* PDFiumJS_GetLayoutAnalsisResults(PDFiumJS_Page *page) {
    printf("PDFLA Called!");
    return nullptr;
}

extern "C"
void PDFiumJS_LayoutAnalsisResults_destroy(PDFiumJS_LayoutAnalysisResult* layoutAnalysisResult) {
    printf("PDFLA Called!");
}