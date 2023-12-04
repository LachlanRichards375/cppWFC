#pragma once
#include <vector>   //std::vector
#include <algorithm> //std::sort
#include "WFCCell.h"
class SortedVector {
private:
    std::vector<WFCCell*> data;
    std::vector<std::vector<WFCCell*>> sortedData;
    size_t vectorSize;
public:
    SortedVector();
    void insert(WFCCell*& value);
    WFCCell* pop();
    WFCCell* popSpecific(WFCPosition* position, int numBits);
    void sort();
    size_t size();
};