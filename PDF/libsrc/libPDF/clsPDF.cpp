#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <pdfiumjs/core/include/fpdfapi/fpdfapi.h>
#include <pdfiumjs/core/include/fxcodec/fx_codec.h>
#pragma GCC diagnostic pop
#include <iostream>
#include <math.h>
#include "clsPDF.h"
#include "LayoutAnalyser.h"

namespace Targoman {
namespace PDF {

namespace Private {

#define MAX_CACHED_PAGES    5

class clsPDFPrivate {
public:
    CPDF_Parser* Parser;
    CPDF_Document* Doc;
    int FirstCachedPageIndex;
    std::vector<CPDF_Page*> CachedPageInternals;
    std::vector<Paragraphs_t> CachedPageParagraphs;
};

class clsPDFPagePrivate {
public:
    CPDF_Document* Doc;
    CPDF_Page* Page;
    Paragraphs_t Lines;
};

class clsPageImagePrivate {
public:
    char* Buffer;
    int W, H;
};

class clsPageTextContentPrivate {
public:
    Paragraphs_t Contents;
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
    if(__builtin_expect(__pdfiumModulesNotInitialized__, false)) {
        initializePdfiumModules();
        __pdfiumModulesNotInitialized__ = false;
    }

    auto Reader = FX_CreateMemoryStream((unsigned char*)_buffer, (size_t)_bufferSize, false);
    this->pPrivate->Parser = new CPDF_Parser;
    this->pPrivate->Parser->StartParse(Reader);
    this->pPrivate->Doc = this->pPrivate->Parser->GetDocument();
    this->pPrivate->FirstCachedPageIndex = -1;
}

clsPDF::~clsPDF()
{
    // Do nothing
}

int clsPDF::getPageCount() const
{
    return this->pPrivate->Doc->GetPageCount();
}

std::vector<CPDF_TextObject*> filterOutRepititions(std::vector<CPDF_TextObject*> _textObjects) {
    return _textObjects;
}

PDFChars_t getPdfTextObjects(CPDF_Page* _page)
{
    auto ObjectPos = _page->GetFirstObjectPosition();
    if(!ObjectPos)
        return PDFChars_t();

    std::vector<CPDF_TextObject*> TextObjects;

    auto Top = _page->GetPageBBox().top;

    int TotalObjectCount = 0;
    while(ObjectPos) {
        auto Object = _page->GetNextObject(ObjectPos);
        ++TotalObjectCount;
        if(Object && Object->m_Type == PDFPAGE_TEXT) {
            CPDF_TextObject* TextObject = (CPDF_TextObject*)Object;
            if(abs(TextObject->m_Left - TextObject->m_Right) < 1e-2f)
                continue;
            TextObjects.push_back(TextObject);
        }
    }

    TextObjects = filterOutRepititions(TextObjects);
    PDFChars_t Characters;

    for(const auto Object : TextObjects) {
        CFX_AffineMatrix M;
        Object->GetTextMatrix(&M);
        PDFFont_t Font = (PDFFont_t)Object->GetFont();
        float Angle = atan2(M.GetB(), M.GetB());
        float FontSize = Object->GetFontSize();
        FX_DWORD SpaceCharCode = Object->GetFont()->CharCodeFromUnicode(' ');
        float MinSpaceWidth;
        if(SpaceCharCode != (FX_DWORD)-1)
            MinSpaceWidth = 0.9 * FontSize * Object->GetFont()->GetCharWidthF(SpaceCharCode) / 1000.0f;
        else
            MinSpaceWidth = FontSize / 4;
        for(int i = 0; i < Object->CountChars(); ++i) {
            CFX_FloatRect R;
            FX_DWORD Code;
            FX_FLOAT Kerning;
            Object->GetCharRect(i, R);
            Object->GetCharInfo(i, Code, Kerning);
            M.TransformRect(R);
            clsPDFChar P(
                        R.left,
                        Top - R.top,
                        R.Width(),
                        R.Height(),
                        MinSpaceWidth,
                        Object->GetFont()->UnicodeFromCharCode(Code).GetAt(0),
                        Angle,
                        Font
            );
            if(Code != 32)
                Characters.push_back(P);
        }
    }

    return Characters;
}

clsPDFPage clsPDF::getPage(const int _pageIndex) const
{
    int NewFirstCachedPageIndex = -1;
    std::vector<CPDF_Page*> NewCachedPageInternals;
    std::vector<Paragraphs_t> NewCachedPageParagraphs;
    for(int Index = _pageIndex - MAX_CACHED_PAGES / 2; Index < _pageIndex + (MAX_CACHED_PAGES + 1) / 2; ++Index) {
        if(Index < 0 || Index >= this->getPageCount())
            continue;
        if(NewFirstCachedPageIndex == -1)
            NewFirstCachedPageIndex = Index;
        int RelativeIndex = Index - this->pPrivate->FirstCachedPageIndex;
        if(this->pPrivate->FirstCachedPageIndex >= 0 && RelativeIndex >= 0 && RelativeIndex < (int)this->pPrivate->CachedPageInternals.size()) {
            NewCachedPageInternals.push_back(this->pPrivate->CachedPageInternals[RelativeIndex]);
            NewCachedPageParagraphs.push_back(this->pPrivate->CachedPageParagraphs[RelativeIndex]);
            this->pPrivate->CachedPageInternals[RelativeIndex] = nullptr;
        } else {
            auto PageDict = this->pPrivate->Doc->GetPage(Index);
            auto Page = new CPDF_Page;
            Page->Load(this->pPrivate->Doc, PageDict);
            Page->ParseContent();
            NewCachedPageInternals.push_back(Page);
            auto PageTextObjects = getPdfTextObjects(Page);
            auto PageParagraphs = LayoutAnalyser::extractParagraphBlocks(PageTextObjects);
            NewCachedPageParagraphs.push_back(PageParagraphs);
        }
    }

    for(auto& UnusedPage : this->pPrivate->CachedPageInternals)
        if(UnusedPage != nullptr)
            delete UnusedPage;

    LayoutAnalyser::updateParagraphBlockTypes(NewCachedPageParagraphs);

    this->pPrivate->FirstCachedPageIndex = NewFirstCachedPageIndex;
    this->pPrivate->CachedPageInternals = NewCachedPageInternals;
    this->pPrivate->CachedPageParagraphs = NewCachedPageParagraphs;

    clsPDFPage Page;
    Page.pPrivate->Doc = this->pPrivate->Doc;
    Page.pPrivate->Page = NewCachedPageInternals[_pageIndex - NewFirstCachedPageIndex];
    Page.pPrivate->Lines = NewCachedPageParagraphs[_pageIndex - NewFirstCachedPageIndex];
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
    PageTextContent.pPrivate->Contents = this->pPrivate->Lines;
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

clsPageTextContent::clsPageTextContent() :
    pPrivate(new Private::clsPageTextContentPrivate)
{
    // Do nothing
}

clsPageTextContent::~clsPageTextContent()
{
    // Do nothing
}

int clsPageTextContent::getParagraphCount() const
{
    return (int)this->pPrivate->Contents.size();
}

const clsParagraph& clsPageTextContent::getParagraph(int _index) const
{
    return this->pPrivate->Contents.at(_index);
}

}
}

extern "C" {

}
