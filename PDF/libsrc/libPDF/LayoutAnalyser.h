#ifndef TARGOMAN_PDF_LAYOUTANALYSER_H
#define TARGOMAN_PDF_LAYOUTANALYSER_H

#include <vector>
#include "clsParagraph.h"

namespace Targoman {
namespace PDF {

typedef void* PDFFont_t;

class clsPDFChar : public clsTextBox {
private:
    wchar_t Code;
    float   Angle;
    PDFFont_t Font;
    float MinSpaceWidth;

public:
    clsPDFChar();
    clsPDFChar(float _x, float _y, float _width, float _height, float _minSpaceWidth, wchar_t _code, float _angle, PDFFont_t _font);

    wchar_t getCode() const { return this->Code; }
    float getAngle() const { return this->Angle; }
    PDFFont_t getFont() const { return this->Font; }
    float getMinSpaceWidth() const { return this->MinSpaceWidth; }
};

typedef std::vector<clsPDFChar> PDFChars_t;

class LayoutAnalyser
{
public:
    static Paragraphs_t extractParagraphBlocks(const PDFChars_t& _pdfChars);
    static void updateParagraphBlockTypes(std::vector<Paragraphs_t>& PageParagraphs);
};

}
}

#endif // TARGOMAN_PDF_LAYOUTANALYSER_H
