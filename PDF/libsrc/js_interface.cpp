#include <libPDF/clsPDF.h>

using namespace Targoman::PDF;

#define RETURN_CLASS_POINTER(PARENT_TYPE, CHILD_TYPE, PARAM_NAME, CALL_FORM) \
    auto result = new CHILD_TYPE(((PARENT_TYPE*)PARAM_NAME)->CALL_FORM); \
    return (void*)result;

extern "C" {

void* clsPDF_create(const char* _buffer, const int _bufferSize)
{
    return (void*)new clsPDF(_buffer, _bufferSize);
}

void clsPDF_delete(void* _this)
{
    delete (clsPDF*)_this;
}

int clsPDF_getPageCount(void* _this)
{
    return ((clsPDF*)_this)->getPageCount();
}

void* clsPDF_getPage(void* _this, int _pageIndex)
{
    RETURN_CLASS_POINTER(clsPDF, clsPDFPage, _this, getPage(_pageIndex))
}

void clsPDF_deletePage(void* _page)
{
    delete (clsPDFPage*)_page;
}

void clsPDF_getBoundingBox(void* _textBox, float* _x, float* _y, float* _width, float* _height)
{
    auto TextBox = (clsTextBox*)_textBox;
    *_x = TextBox->getX();
    *_y = TextBox->getY();
    *_width = TextBox->getWidth();
    *_height = TextBox->getHeight();
}

void* clsPDF_getTextContents(void* _page)
{
    RETURN_CLASS_POINTER(clsPDFPage, clsPageTextContent, _page, getTextContents())
}

void clsPDF_deleteTextContents(void* _object)
{
    delete (clsPageTextContent*)_object;
}

void* clsPDF_getRenderedImage(void* _page)
{
    RETURN_CLASS_POINTER(clsPDFPage, clsPageImage, _page, getRenderedImage())
}

void clsPDF_deleteRenderedImage(void* _object)
{
    delete (clsPageImage*)_object;
}

char* clsPDF_getBuffer(void* _renderedImage)
{
    return ((clsPageImage*)_renderedImage)->getBuffer();
}

int clsPDF_getWidth(void* _renderedImage)
{
    return ((clsPageImage*)_renderedImage)->getWidth();
}

int clsPDF_getHeight(void* _renderedImage)
{
    return ((clsPageImage*)_renderedImage)->getHeight();
}

int clsPDF_getParagraphCount(void* _pageTextContent)
{
    return ((clsPageTextContent*)_pageTextContent)->getParagraphCount();
}

void* clsPDF_getParagraph(void* _pageTextContent, int _parIndex)
{
    return (void*)&(((clsPageTextContent*)_pageTextContent)->getParagraph(_parIndex));
}

void clsPDF_deleteParagraph(void* _object)
{
    // Do nothing
}

int clsPDF_getParagraphType(void* _paragraph)
{
    return ((clsParagraph*)_paragraph)->getType();
}

const wchar_t* clsPDF_getContentString(void* _paragraph)
{
    return ((clsParagraph*)_paragraph)->getContentString().data();
}

int clsPDF_getSentenceCount(void* _paragraph)
{
    return ((clsParagraph*)_paragraph)->getContents().size();
}

void* clsPDF_getSentence(void* _paragraph, int _sentIndex)
{
    return (void*)&(((clsParagraph*)_paragraph)->getContents().at(_sentIndex));
}

void clsPDF_deleteSentence(void* _object)
{
    // Do nothing
}

int clsPDF_getLineCount(void* _sentence)
{
    return ((Sentence_t*)_sentence)->size();
}

void* clsPDF_getLine(void* _sentence, int _lineIndex)
{
    return (void*)&(((Sentence_t*)_sentence)->at(_lineIndex));
}

void clsPDF_deleteLine(void* _line)
{
    // Do nothing
}

const wchar_t* clsPDF_getLineContents(void* _line)
{
    return ((clsLine*)_line)->getContents().data();
}

}
