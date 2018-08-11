#ifndef TARGOMAN_PDF_CLSPDF_H
#define TARGOMAN_PDF_CLSPDF_H

#include <memory>
#include "clsParagraph.h"

namespace Targoman {
namespace PDF {

namespace Private {
class clsPDFPrivate;
class clsPDFPagePrivate;
class clsPageImagePrivate;
class clsPageTextContentPrivate;
}

class clsPDFPage;


class clsPageTextContent {
private:
    std::shared_ptr<Private::clsPageTextContentPrivate> pPrivate;

public:
    clsPageTextContent(clsPageTextContent&& _other);
    clsPageTextContent();
    ~clsPageTextContent();

    int getParagraphCount() const;
    const clsParagraph& getParagraph(int _index) const;

public:
    friend class clsPDFPage;
};

class clsPageImage {
private:
    std::shared_ptr<Private::clsPageImagePrivate> pPrivate;

public:
    clsPageImage(clsPageImage&& _other);
    clsPageImage();
    ~clsPageImage();

    char* getBuffer();
    int getHeight() const;
    int getWidth() const;

public:
    friend class clsPDFPage;
};

class clsPDFPage {
private:
    std::shared_ptr<Private::clsPDFPagePrivate> pPrivate;

public:
    clsPDFPage(clsPDFPage&& _other);
    clsPDFPage();
    ~clsPDFPage();

    clsPageTextContent getTextContents() const;
    clsPageImage getRenderedImage() const;

public:
    friend class clsPDF;
};

class clsPDF {
private:
    std::shared_ptr<Private::clsPDFPrivate> pPrivate;

public:
    clsPDF(clsPDF&& _other);
    clsPDF(const char* _buffer, const int _bufferSize);
    ~clsPDF();

    int getPageCount() const;
    clsPDFPage getPage(const int _pageIndex) const;
};

}
}

extern "C" {

}

#endif // TARGOMAN_PDF_CLSPDF_H
