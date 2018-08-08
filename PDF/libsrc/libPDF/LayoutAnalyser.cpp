#include <algorithm>
#include <tuple>
#include <utility>
#include "LayoutAnalyser.h"

namespace Targoman {
namespace PDF {

clsPDFChar::clsPDFChar() : clsPDFChar(0, 0, 0, 0, 0, 0, 0, NULL)
{ }

clsPDFChar::clsPDFChar(float _x, float _y, float _width, float _height, float _minSpaceWidth, wchar_t _code, float _angle, PDFFont_t _font)
    : clsTextBox(_x, _y, _width, _height),
      Code(_code), Angle(_angle), Font(_font), MinSpaceWidth(_minSpaceWidth)
{ }

template<typename ItemType_t, typename GetValue_t>
double mean(std::vector<ItemType_t> items, GetValue_t get) {
    double result = 0;
    for(auto item : items)
        result += get(item);
    return result / (double)items.size();
}

template<typename ItemType_t, typename GetValue_t>
std::vector<std::pair<double, double>> getEmptySpaces(std::vector<ItemType_t> items, GetValue_t get) {
    typedef std::pair<double, double> Range_t;
    std::vector<Range_t> OccupiedSpaces;
    double Min, Max;
    std::tie(Min, Max) = get(items[0]);
    for(auto item : items) {
        double P0, P1;
        std::tie(P0, P1) = get(item);
        OccupiedSpaces.push_back(Range_t(P0, P1));
        if(Min > P0)
            Min = P0;
        if(Max < P1)
            Max = P1;
    }
    std::sort(
        OccupiedSpaces.begin(),
        OccupiedSpaces.end(),
        [](Range_t& a, Range_t& b) {
            return a.first < b.first;
        }
    );

    std::vector<Range_t> Result;
    double p1 = OccupiedSpaces[0].second;
    for(int i = 0; i < (int)OccupiedSpaces.size() - 1; ++i) {
        double delta = OccupiedSpaces[i + 1].first - p1;
        if(delta > 0)
            Result.push_back(Range_t(p1, OccupiedSpaces[i + 1].first));
        p1 = std::max(p1, OccupiedSpaces[i + 1].second);
    }

    return Result;
}

std::vector<PDFFont_t> getUsedFonts(float y0, float y1, PDFChars_t pdfItems) {
    std::vector<void*> result;
    for(auto& item : pdfItems) {
        float yy0 = std::max(item.getTop(), y0);
        float yy1 = std::min(item.getBottom(), y1);
        if(yy0 < yy1)
            result.push_back(item.getFont());
    }
    return result;
}

std::vector<PDFChars_t> splitVertically(const PDFChars_t& _chars)
{
    std::vector<PDFChars_t> Result;

    double meanCharHeight = mean(_chars, [](clsPDFChar x) { return x.getHeight(); });
    auto vacantSpaces = getEmptySpaces(_chars, [](clsPDFChar x) { return std::pair<double, double>(x.getTop(), x.getBottom()); });
    double meanEmptyHeight = mean(vacantSpaces, [](std::pair<double, double> x) { return x.second - x.first; });

    if(vacantSpaces.size() > 3) {
        meanEmptyHeight *= 1.5;
    }

    double threshold = std::max(meanCharHeight, meanEmptyHeight);
    std::vector<double> breakPositions;
    for(auto& item : vacantSpaces) {
        double delta = item.second - item.first;
        if(delta < 0)
            printf("%s(%d): WARNING delta=%lg\n", __FILE__, __LINE__, delta);
        if(delta > threshold)
            breakPositions.push_back((item.second + item.first) / 2);
        else {
            std::vector<void*> usedFontsAbove, usedFontsBelow;
            usedFontsAbove = getUsedFonts(item.first - meanCharHeight, item.first, _chars);
            usedFontsBelow = getUsedFonts(item.second, item.second + meanCharHeight, _chars);
            bool coincide = false;
            for(auto& f0 : usedFontsAbove) {
                for(auto& f1 : usedFontsBelow)
                    if(f0 == f1) {
                        coincide = true;
                        break;
                    }
                if(coincide)
                    break;
            }
            if(!coincide)
                breakPositions.push_back((item.second + item.first) / 2);
        }
    }

    Result.resize(breakPositions.size() + 1);
    for(auto& item : _chars) {
        int index = 0;
        while(index < (int)breakPositions.size() && item.getY() > breakPositions[index])
            ++index;
        Result[index].push_back(item);
    }

    return Result;
}

std::vector<PDFChars_t> splitHorizontally(const PDFChars_t& pdfItems) {
    std::vector<PDFChars_t> Result;

    double meanCharHeight = mean(pdfItems, [](clsPDFChar x) { return x.getHeight(); });
    auto vacantSpaces = getEmptySpaces(pdfItems, [](clsPDFChar x) { return std::pair<double, double>(x.getLeft(), x.getRight()); });
    double meanEmptyWidth = mean(vacantSpaces, [](std::pair<double, double> x) { return x.second - x.first; });

    double threshold = std::max(1.5 * meanCharHeight, 0.95 * meanEmptyWidth);
    std::vector<double> breakPositions;
    for(auto& item : vacantSpaces) {
        double delta = item.second - item.first;
        if(delta < 0)
            printf("%s(%d): WARNING delta=%lg\n", __FILE__, __LINE__, delta);
        if(delta > threshold)
            breakPositions.push_back((item.second + item.first) / 2);
    }

    Result.resize(breakPositions.size() + 1);
    for(auto& item : pdfItems) {
        int index = 0;
        while(index < (int)breakPositions.size() && item.getX() > breakPositions[index])
            ++index;
        Result[index].push_back(item);
    }

    return Result;
}

typedef std::vector<PDFChars_t> (*SplitFunction_t)(const PDFChars_t& pdfItems);

std::vector<PDFChars_t> groupLineItems(const PDFChars_t &_pdfChars)
{
    auto allItems = _pdfChars;
    std::sort(
        allItems.begin(),
        allItems.end(),
        [] (const clsPDFChar& a, const clsPDFChar& b) {
            if(a.verticallOverlap(b) < 1e-3)
                return a.getTop() < b.getTop();
            return a.getLeft() < b.getLeft();
        }
    );
    auto emptySpaces = getEmptySpaces(allItems, [](clsPDFChar x) { return std::pair<float, float>(x.getTop(), x.getBottom()); });
    std::vector<float> wallPositions;
    wallPositions.resize(emptySpaces.size());
    std::transform(emptySpaces.begin(), emptySpaces.end(), wallPositions.begin(), [](std::pair<double, double> _space) { return 0.5 * (_space.first + _space.second); });
    std::vector<PDFChars_t> Result;
    Result.resize(wallPositions.size() + 1);
    for(auto& Char : _pdfChars) {
        int position = 0;
        for(; position < (int)wallPositions.size(); ++position)
            if(Char.getTop() < wallPositions[position])
                break;
        Result[position].push_back(Char);
    }
    return Result;
}

float calculateSpaceThreshold(const PDFChars_t &_pdfChars)
{
    if(_pdfChars.size() == 0)
        return 0;
    float MinSpaceWidth = _pdfChars.at(0).getMinSpaceWidth();
    for(size_t i = 1; i < _pdfChars.size(); ++i)
        MinSpaceWidth = std::min(MinSpaceWidth, _pdfChars[i].getMinSpaceWidth());
    auto allItems = _pdfChars;
    std::sort(
        allItems.begin(),
        allItems.end(),
        [] (const clsPDFChar& a, const clsPDFChar& b) {
            return a.getLeft() < b.getLeft();
        }
    );
    float Threshold = 0;
    for(int i = 1; i < (int)allItems.size(); ++i) {
        float delta = std::max(0.0f, allItems[i].getLeft() - allItems[i - 1].getRight());
        Threshold += delta;
    }
    Threshold /= (allItems.size() - 1);
    return std::max(1.5f * Threshold, MinSpaceWidth);
}

const std::wstring __sentenceEnderCharacters__ = L".!?;.؟!؛";

Paragraphs_t LayoutAnalyser::extractParagraphBlocks(const PDFChars_t &_pdfChars)
{
    std::vector<PDFChars_t> Blocks = { _pdfChars };

    // Find text blocks inside the document page
    SplitFunction_t doSplit = splitVertically;
    while(true) {
        std::vector<PDFChars_t> UpdatedBlocks;
        for(auto& Block: Blocks) {
            std::vector<PDFChars_t> SplitResults = doSplit(Block);
            UpdatedBlocks.insert(UpdatedBlocks.end(), SplitResults.begin(), SplitResults.end());
        }
        if(doSplit != splitVertically && UpdatedBlocks.size() == Blocks.size())
            break;
        Blocks = UpdatedBlocks;
        if(doSplit == splitVertically)
            doSplit = splitHorizontally;
        else
            doSplit = splitVertically;
    }

    // Extract paragraphs inside document page
    Paragraphs_t Paragraphs;
    for(auto& Block : Blocks) {
        clsParagraph P;
        auto CharsInLines = groupLineItems(Block);
        Sentence_t Sentence;
        for(PDFChars_t& LineChars : CharsInLines) {
            auto SpaceThreshold = calculateSpaceThreshold(LineChars);
            clsLine CurrentLine;
            for(int i = 0; i < (int)LineChars.size(); ++i) {
                CurrentLine.unionBy(LineChars[i]);
                CurrentLine.getContents().push_back(LineChars[i].getCode());
                bool BreakCandidate = i == (int)LineChars.size() - 1;
                bool SpaceInserted = false;
                if(i < (int)LineChars.size() - 1) {
                    float delta = LineChars[i + 1].getLeft() - LineChars[i].getRight();
                    if(delta > SpaceThreshold) {
                        BreakCandidate = true;
                        SpaceInserted = true;
                        CurrentLine.getContents().push_back(32);
                    }
                }
                if(BreakCandidate && __sentenceEnderCharacters__.find(LineChars[i].getCode()) != std::wstring::npos) {
                    if(SpaceInserted)
                        CurrentLine.getContents().erase(CurrentLine.getContents().size() - 1);
                    Sentence.push_back(CurrentLine);
                    P.addSentence(Sentence);
                    Sentence.clear();
                    CurrentLine.clear();
                }
            }
            if(CurrentLine.getContents().size() > 0)
                Sentence.push_back(CurrentLine);
        }
        if(Sentence.size())
            P.addSentence(Sentence);
        Paragraphs.push_back(P);
    }

    return Paragraphs;
}

void LayoutAnalyser::updateParagraphBlockTypes(std::vector<Paragraphs_t> &PageParagraphs)
{
//    struct LineSpecs {
//        int PageIndex, ParIndex, SentIndex, LineIndex;

//        LineSpecs(int _pageIndex, int _parIndex, int _sentIndex, int _lineIndex) {
//            this->PageIndex = _pageIndex;
//            this->ParIndex = _parIndex;
//            this->SentIndex = _sentIndex;
//            this->LineIndex = _lineIndex;
//        }

//        LineSpecs(const LineSpecs& _other) {
//            this->PageIndex = _other.PageIndex;
//            this->ParIndex = _other.ParIndex;
//            this->SentIndex = _other.SentIndex;
//            this->LineIndex = _other.LineIndex;
//        }

//        bool operator == (const LineSpecs& _other) {
//            return
//                this->PageIndex == _other.PageIndex &&
//                this->ParIndex == _other.ParIndex &&
//                this->SentIndex == _other.SentIndex &&
//                this->LineIndex == _other.LineIndex;
//        }
//    };

//    std::vector<LineSpecs> HeaderFooterCandidates;
    for(int iii = 0; iii < (int)PageParagraphs.size() - 1; ++iii) {
        for(int jjj = iii + 1; jjj < (int)PageParagraphs.size(); ++jjj) {
            for(int i = 0; i < (int)PageParagraphs[iii].size(); ++i) {
                const Sentences_t& C0 = PageParagraphs[iii][i].getContents();
                for(int j = 0; j < (int)PageParagraphs[jjj].size(); ++j) {
                    const Sentences_t& C1 = PageParagraphs[jjj][j].getContents();
                    for(int k = 0; k < (int)C0.size(); ++k) {
                        const Sentence_t& S0 = C0[k];
                        for(int l = 0; l < (int)C1.size(); ++l) {
                            const Sentence_t& S1 = C1[l];
                            for(int m = 0; m < (int)S0.size(); ++m) {
                                for(int n = 0; n < (int)S1.size(); ++n) {
                                    if(S0[m].areSameBoxes(S1[n]) == false)
                                        continue;
                                    if(S0[m].getContents() != S1[n].getContents())
                                        continue;
//                                    HeaderFooterCandidates.push_back();
                                    PageParagraphs[iii][i].setType(enuParagraphType::Header);
                                    PageParagraphs[jjj][j].setType(enuParagraphType::Header);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

}
}
