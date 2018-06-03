#ifndef __PDFIUMJS_LAYOUT_ANALYSIS_H
#define __PDFIUMJS_LAYOUT_ANALYSIS_H

#include <utility>
#include <vector>

namespace enuPdfItemType {
    enum Type {
        Character = 0,
        Line = 1
    };
}

struct stuPdfItem {
    enuPdfItemType::Type Type;
    double X, Y, Width, Height;
    int Code;
    double Angle;
    void* Font;
    double FontSize;    
};
typedef std::vector<stuPdfItem> PageBlock_t;

std::vector<PageBlock_t> analyseLayout(const PageBlock_t& pdfItems);

#endif
