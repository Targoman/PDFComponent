// Copyright (c) 2018 Targoman Intelligent Processing Co.

#include "./pdfium.js.h"
#include "./layout_analysis.h"
#include "../core/include/fpdfapi/fpdf_page.h"
#include "../core/include/reflow/fpdf_layout.h"
#include "../core/include/fpdfapi/fpdf_pageobj.h"

struct PDFiumJS_LayoutAnalysisResult {
    int CharCount;
    double* Left;
    double* Top;
    double* Right;
    double* Bottom;
    int* Code;
    double* Angle;
    void** Font;
    double* FontSize;
};

void getBlockBounds(PageBlock_t block, double& l, double& b, double& r, double& t) {
    l = block[0].X;
    b = block[0].Y;
    r = l + block[0].Width;
    t = b + block[0].Height;
    for(auto& item : block) {
        double ll = item.X;
        double bb = item.Y;
        double rr = ll + item.Width;
        double tt = bb + item.Height;
        if(ll < l)
            l = ll;
        if(bb < b)
            b = bb;
        if(rr > r)
            r = rr;
        if(tt > t)
            t = tt;
    }
}

extern "C"
PDFiumJS_LayoutAnalysisResult* PDFiumJS_GetLayoutAnalsisResults(PDFiumJS_Page *_doc) {

    CPDF_Page* page = (CPDF_Page*)_doc->page;
    double pageHeight = page->GetPageBBox().top;

    int objectCount = page->CountObjects();
    for(int i = 0; i < objectCount; ++i) {
        auto object = page->GetObjectByIndex(i);
        if(object->m_Type == PDFPAGE_PATH) {
            CPDF_PathObject* path = (CPDF_PathObject*)object;
            int pointCount = path->m_Path.GetPointCount();
            auto points = path->m_Path.GetPoints();
            printf("%s(%d): PATH (%s Rect)\n", __FILE__, __LINE__, path->m_Path.IsRect() ? "Is" : "Is NOT");
            for(int j = 0; j < pointCount; ++j) {
                auto p = points[j];
                printf("%s(%d): X: %f, Y:%f\n", __FILE__, __LINE__, p.m_PointX, p.m_PointY);
            }
            printf("%s(%d): END PATH\n", __FILE__, __LINE__);
        }
    }

    FPDF_TEXTPAGE TextPage = FPDFText_LoadPage(_doc->page);
    PDFiumJS_LayoutAnalysisResult* Result = new PDFiumJS_LayoutAnalysisResult;

    PageBlock_t WholePage;
    int CharCount = FPDFText_CountChars(TextPage);
    WholePage.reserve(CharCount);

    for(int i = 0; i < CharCount; ++i) {
        stuPdfItem Item;
        Item.Type = enuPdfItemType::Character;
        FPDFText_GetCharBox(TextPage, i, &Item.X, &Item.Width, &Item.Y, &Item.Height);
        Item.Width -= Item.X;
        auto RealTop = pageHeight - Item.Height;
        Item.Height -= Item.Y;
        Item.Y = RealTop;
        Item.Code = FPDFText_GetUnicode(TextPage, i);
        FPDFText_GetInfo(TextPage, i, &Item.Font, &Item.FontSize, &Item.Angle);
        if(abs(Item.Angle) < 1e-4 && Item.Code != 32)
            WholePage.push_back(Item);
    }

    std::vector<PageBlock_t> R = analyseLayout(WholePage);
    // printf("%s(%d): rec_block_count=%d\n", __FILE__, __LINE__, R.size());

    // Result->CharCount = CharCount;
    Result->CharCount = (int)R.size();
    Result->Left = new double[Result->CharCount];
    Result->Top = new double[Result->CharCount];
    Result->Right = new double[Result->CharCount];
    Result->Bottom = new double[Result->CharCount];
    Result->Code = new int[Result->CharCount];
    Result->Angle = new double[Result->CharCount];
    Result->Font = new void*[Result->CharCount];
    Result->FontSize = new double[Result->CharCount];

    for(int i = 0; i < Result->CharCount; ++i) {
        // FPDFText_GetCharBox(
        //     TextPage,
        //     i,
        //     Result->Left + i,
        //     Result->Right + i,
        //     Result->Bottom + i,
        //     Result->Top + i
        // );
        // Result->Code[i] = FPDFText_GetUnicode(TextPage, i);
        // FPDFText_GetInfo(TextPage, i, Result->Font + i, Result->FontSize + i, Result->Angle + i);
        getBlockBounds(R[i], Result->Left[i], Result->Top[i], Result->Right[i], Result->Bottom[i]);
        Result->Font[i] = NULL;
        Result->FontSize[i] = 1.0;
        Result->Angle[i] = 0.0;
    }
    return Result;
}

extern "C"
void PDFiumJS_LayoutAnalsisResults_destroy(PDFiumJS_LayoutAnalysisResult* _result) {
    // printf("%s(%d): Result: %p\n", __FILE__, __LINE__, _result);
    delete[] _result->Left;
    delete[] _result->Top;
    delete[] _result->Right;
    delete[] _result->Bottom;
    delete[] _result->Code;
    delete _result;
}