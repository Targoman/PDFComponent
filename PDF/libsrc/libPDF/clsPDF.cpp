#include "clsPDF.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <pdfiumjs/core/include/fpdfapi/fpdfapi.h>
#include <pdfiumjs/core/include/fxcodec/fx_codec.h>
#pragma GCC diagnostic pop
#include <iostream>

namespace Targoman {
namespace PDF {

namespace Private {

class clsPDFPrivate {
public:
    CPDF_Parser* Parser;
    CPDF_Document* Doc;
};

class clsPDFPagePrivate {
public:
    CPDF_Document* Doc;
    CPDF_Page* Page;
};

class clsPageImagePrivate {
public:
    char* Buffer;
    int W, H;
};

class clsPageTextContentPrivate {

};

}

bool __pdfiumModulesNotInitialized__ = true;
inline void initializePdfiumModules()
{
    CPDF_ModuleMgr::Create();
    CFX_GEModule::Create();

    auto Codec = CCodec_ModuleMgr::Create();
    CFX_GEModule::Get()->SetCodecModule(Codec);
    CPDF_ModuleMgr::Get()->SetCodecModule(Codec);

    CPDF_ModuleMgr::Get()->InitPageModule();
    CPDF_ModuleMgr::Get()->InitRenderModule();
}

clsPDF::clsPDF(clsPDF &&_other)
{
    this->pPrivate.swap(_other.pPrivate);
}

clsPDF::clsPDF(const char *_buffer, const int _bufferSize) :
    pPrivate(new Private::clsPDFPrivate)
{
    if(Q_UNLIKELY(__pdfiumModulesNotInitialized__)) {
        initializePdfiumModules();
        __pdfiumModulesNotInitialized__ = false;
    }

    auto Reader = FX_CreateMemoryStream((unsigned char*)_buffer, (size_t)_bufferSize, false);
    this->pPrivate->Parser = new CPDF_Parser;
    this->pPrivate->Parser->StartAsynParse(Reader);
    this->pPrivate->Doc = this->pPrivate->Parser->GetDocument();
}

clsPDF::~clsPDF()
{
    // Do nothing
}

int clsPDF::getPageCount() const
{
    return this->pPrivate->Doc->GetPageCount();
}

clsPDFPage clsPDF::getPage(const int _pageIndex) const
{
    clsPDFPage Page;
    auto PageDict = this->pPrivate->Doc->GetPage(_pageIndex);
    Page.pPrivate->Doc = this->pPrivate->Doc;
    Page.pPrivate->Page = new CPDF_Page;
    Page.pPrivate->Page->Load(this->pPrivate->Doc, PageDict);
    Page.pPrivate->Page->ParseContent();
    return Page;
}

clsPageImage::clsPageImage(clsPageImage &&_other)
{
    this->pPrivate.swap(_other.pPrivate);
}

clsPageImage::clsPageImage() : pPrivate(new Private::clsPageImagePrivate) {
    // Do nothing
}

clsPageImage::~clsPageImage()
{
    // Do nothing
}

char *clsPageImage::getBuffer()
{
    return this->pPrivate->Buffer;
}

int clsPageImage::getHeight() const
{
    return this->pPrivate->H;
}

int clsPageImage::getWidth() const
{
    return this->pPrivate->W;
}

clsPDFPage::clsPDFPage(clsPDFPage &&_other)
{
    this->pPrivate.swap(_other.pPrivate);
}

clsPDFPage::clsPDFPage() :
    pPrivate(new Private::clsPDFPagePrivate)
{
    // Do nothing!
}

clsPDFPage::~clsPDFPage()
{
    // Do nothing
}

clsPageTextContent clsPDFPage::getTextContents() const
{
    clsPageTextContent PageTextContent;

    auto ObjectPos = this->pPrivate->Page->GetFirstObjectPosition();
    if(!ObjectPos)
        return PageTextContent;
}

clsPageImage clsPDFPage::getRenderedImage() const
{
    auto& Page = this->pPrivate->Page;
    int W = Page->GetPageWidth(), H = Page->GetPageHeight();
    auto Bmp = new CFX_DIBitmap;
    Bmp->Create(W, H, FXDIB_Rgb);
    FX_RECT WholePageRect(0, 0, W, H);

    CFX_FxgeDevice Device;
    Device.Attach(Bmp);
    Device.FillRect(&WholePageRect, 0xffffffff);

    CFX_AffineMatrix Matrix;
    Page->GetDisplayMatrix(Matrix, 0, 0, W, H, 0);
    auto Context = new CPDF_RenderContext();
    Context->Create(Page);
    Context->AppendObjectList(Page, &Matrix);
    auto Renderer = new CPDF_ProgressiveRenderer;
    Renderer->Start(Context, &Device, NULL, NULL);

    clsPageImage PageImage;
    PageImage.pPrivate->Buffer = new char[3 * W * H];
    PageImage.pPrivate->W = W;
    PageImage.pPrivate->H = H;
    for(int i = 0; i < H; ++i) {
        auto ScanLine = Bmp->GetScanline(i);
        std::cout << (int)ScanLine[0] << std::endl;
        for(int j = 0; j < 3 * W; ++j) {
            PageImage.pPrivate->Buffer[3 * W * i + j] = (char)ScanLine[j];
        }
    }

    return PageImage;
}

clsPageTextContent::clsPageTextContent(clsPageTextContent &&_other)
{
    this->pPrivate.swap(_other.pPrivate);
}

clsPageTextContent::~clsPageTextContent()
{
    // Do nothing
}

}
}
