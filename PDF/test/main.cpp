#include <libPDF/clsPDF.h>
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace Targoman::PDF;

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

//    std::ifstream pdfFile("/home/behrooz/Downloads/Part 1 Beginnings-Part 2 Designing for the senses.PDF");
    std::ifstream pdfFile("/home/behrooz/a121.pdf");
//    std::ifstream pdfFile("/home/behrooz/Khadivi_ACL06-translate.pdf");
    pdfFile.seekg(0, std::ios_base::end);
    int BufferSize = (int)pdfFile.tellg();
    pdfFile.seekg(0, std::ios_base::beg);
    char* Buffer = new char[BufferSize];
    pdfFile.read(Buffer, BufferSize);
    pdfFile.close();

    clsPDF PDF(Buffer, BufferSize);
    std::cout << "PDF has " << PDF.getPageCount() << " pages in total." << std::endl;

    int PageIndex = 2;
    auto Page = PDF.getPage(PageIndex);
    auto PageImage = Page.getRenderedImage();

    delete[] Buffer;

    cv::Mat OutputImage(PageImage.getHeight(), PageImage.getWidth(), CV_8UC3);

    Buffer = PageImage.getBuffer();
    for(int y = 0; y < PageImage.getHeight(); ++y)
        for(int x = 0; x < PageImage.getWidth(); ++x)
        {
            cv::Vec3b C;
            for(int j = 0; j < 3; ++j)
                C[j] = (uchar)Buffer[3 * (y * PageImage.getWidth() + x) + j];
            OutputImage.at<cv::Vec3b>(cv::Point(x, y)) = C;
        }

    auto TextContent = Page.getTextContents();

    cv::imwrite("./without_chars.bmp", OutputImage);

    for(int i = 0; i < TextContent.getParagraphCount(); ++i) {
        auto& P = TextContent.getParagraph(i);
        if(P.getType() != enuParagraphType::Header)
            continue;
        std::cout << "===============================================================" << std::endl;
        std::cout << "X: " << P.getX() << ", Y: " << P.getY() << ", W: " << P.getWidth() << ", H: " << P.getHeight() << std::endl;
        std::cout << "Type: " << P.getType() << std::endl;
        cv::rectangle(OutputImage, cv::Rect((int)P.getX(), (int)P.getY(), (int)P.getWidth(), (int)P.getHeight()), cv::Scalar(0, 0, 0), 1);
    }

    cv::imwrite("./with_chars.bmp", OutputImage);
    std::cout << "PDF Page " << PageIndex << ": " << PageImage.getWidth() << " x " << PageImage.getHeight() << " at " << (void*)PageImage.getBuffer() << std::endl;

}
