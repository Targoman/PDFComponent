#include <algorithm>
#include <tuple>
#include <utility>
#include <vector>
#include "./layout_analysis.h"
#include <stdio.h>

typedef std::vector<PageBlock_t> (*SplitFunction_t)(const PageBlock_t& pdfItems);

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

std::vector<void*> getUsedFonts(double y0, double y1, PageBlock_t pdfItems) {
    std::vector<void*> result;
    for(auto& item : pdfItems) {
        double yy0 = std::max(item.Y, y0);
        double yy1 = std::min(item.Y + item.Height, y1);
        if(yy0 < yy1)
            result.push_back(item.Font);
    }
    return result;
}

std::vector<PageBlock_t> splitVertically(const PageBlock_t& pdfItems) {
    std::vector<PageBlock_t> Result;

    double meanCharHeight = mean(pdfItems, [](stuPdfItem x) { return x.Height; });
    auto vacantSpaces = getEmptySpaces(pdfItems, [](stuPdfItem x) { return std::pair<double, double>(x.Y, x.Y + x.Height); });
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
            usedFontsAbove = getUsedFonts(item.first - meanCharHeight, item.first, pdfItems);
            usedFontsBelow = getUsedFonts(item.second, item.second + meanCharHeight, pdfItems);
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
    for(auto& item : pdfItems) {
        int index = 0;
        while(index < (int)breakPositions.size() && item.Y > breakPositions[index])
            ++index;
        Result[index].push_back(item);
    }

    return Result;
}

std::vector<PageBlock_t> splitHorizontally(const PageBlock_t& pdfItems) {
    std::vector<PageBlock_t> Result;

    double meanCharHeight = mean(pdfItems, [](stuPdfItem x) { return x.Height; });
    auto vacantSpaces = getEmptySpaces(pdfItems, [](stuPdfItem x) { return std::pair<double, double>(x.X, x.X + x.Width); });
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
        while(index < (int)breakPositions.size() && item.X > breakPositions[index])
            ++index;
        Result[index].push_back(item);
    }

    return Result;
}

std::vector<PageBlock_t> analyseLayout(const PageBlock_t& pdfItems) {
    std::vector<PageBlock_t> Blocks = { pdfItems };
    SplitFunction_t doSplit = splitVertically;
    while(true) {
        std::vector<PageBlock_t> UpdatedBlocks;
        for(auto& Block: Blocks) {
            std::vector<PageBlock_t> SplitResults = doSplit(Block);
            UpdatedBlocks.insert(UpdatedBlocks.end(), SplitResults.begin(), SplitResults.end());
        }
        if(doSplit != splitVertically && UpdatedBlocks.size() == Blocks.size())
            break;
        printf("%s(%d): updated_block_count=%d\n", __FILE__, __LINE__, UpdatedBlocks.size());
        Blocks = UpdatedBlocks;
        if(doSplit == splitVertically)
            doSplit = splitHorizontally;
        else
            doSplit = splitVertically;
    }
    printf("%s(%d): block_count=%d\n", __FILE__, __LINE__, Blocks.size());
    return Blocks;
}