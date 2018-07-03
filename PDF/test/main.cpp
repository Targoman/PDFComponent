#include <libPDF/clsPDF.h>
#include <iostream>
#include <fstream>

int main() {
//    CPDF_ModuleMgr::Create();
//    CFX_GEModule::Create();

//    auto Codec = CCodec_ModuleMgr::Create();
//    CFX_GEModule::Get()->SetCodecModule(Codec);
//    CPDF_ModuleMgr::Get()->SetCodecModule(Codec);

//    CPDF_ModuleMgr::Get()->InitPageModule();
//    CPDF_ModuleMgr::Get()->InitRenderModule();

//    std::cout << "Hello world!" << std::endl;

//    auto Reader = FX_CreateFileRead("/home/behrooz/Downloads/Part 1 Beginnings-Part 2 Designing for the senses.PDF");
//    auto Parser = new CPDF_Parser;
//    Parser->StartAsynParse(Reader);
//    auto Doc = Parser->GetDocument();
//    auto PageCount = Doc->GetPageCount();

//    std::cout << "The document has " << PageCount << " pages in total." << std::endl;

//    auto PageDict = Doc->GetPage(1);
//    auto Page = new CPDF_Page;
//    Page->Load(Doc, PageDict);
//    Page->ParseContent();
//    auto w = Page->GetPageWidth();
//    auto h = Page->GetPageHeight();
//    auto Bmp = new CFX_DIBitmap;
//    Bmp->Create(w, h, FXDIB_Rgb32);
//    FX_RECT WholeRect(0, 0, w, h);
//    CFX_FxgeDevice Device;
//    Device.Attach(Bmp);
//    Device.FillRect(&WholeRect, 0xffffff);
//    CFX_AffineMatrix Matrix;
//    Page->GetDisplayMatrix(Matrix, 0, 0, w, h, 0);
//    auto Context = new CPDF_RenderContext;
//    Context->Create(Page);
//    Context->AppendObjectList(Page, &Matrix);
//    auto Renderer = new CPDF_ProgressiveRenderer;
//    Renderer->Start(Context, &Device, NULL, NULL);

//    std::cout << "Page 1 rendered successfully!" << std::endl;

    std::ifstream pdfFile("/home/behrooz/Downloads/Part 1 Beginnings-Part 2 Designing for the senses.PDF");
    pdfFile.seekg(0, std::ios_base::end);
    int BufferSize = (int)pdfFile.tellg();
    pdfFile.seekg(0, std::ios_base::beg);
    char* Buffer = new char[BufferSize];
    pdfFile.read(Buffer, BufferSize);
    pdfFile.close();

    Targoman::PDF::clsPDF PDF(Buffer, BufferSize);
    std::cout << "PDF has " << PDF.getPageCount() << " pages in total." << std::endl;

    auto Page = PDF.getPage(0);
    auto PageImage = Page.getRenderedImage();

    std::cout << "PDF Page 0: " << PageImage.getWidth() << " x " << PageImage.getHeight() << " at " << (void*)PageImage.getBuffer() << std::endl;
}
