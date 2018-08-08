#ifndef TARGOMAN_PDF_CLSPDF_H
#define TARGOMAN_PDF_CLSPDF_H

#include <QtCore>
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
    QScopedPointer<Private::clsPageTextContentPrivate> pPrivate;

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
    QScopedPointer<Private::clsPageImagePrivate> pPrivate;
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
    QScopedPointer<Private::clsPDFPagePrivate> pPrivate;

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
    QScopedPointer<Private::clsPDFPrivate> pPrivate;

public:
    clsPDF(clsPDF&& _other);
    clsPDF(const char* _buffer, const int _bufferSize);
    ~clsPDF();

    int getPageCount() const;
    clsPDFPage getPage(const int _pageIndex) const;
};

}
}


#endif // TARGOMAN_PDF_CLSPDF_H
