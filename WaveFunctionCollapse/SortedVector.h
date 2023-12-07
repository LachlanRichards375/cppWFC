#pragma once
#include <vector>   //std::vector
#include <algorithm> //std::sort
#include "WFCCell.h"
class SortedVector {
private:
    std::vector<std::vector<WFCCell*>> sortedData;
    //[bitsInDomain][numElements] = indexInSortedData that needs to be moved
    std::vector<std::vector<int>> dirtyData;
    size_t vectorSize;
public:
    SortedVector();
    void insert(WFCCell*& value);
    WFCCell* pop();
    WFCCell* popSpecific(WFCPosition* position, int numBits);
    void sort();
    void SetDirty(WFCCell* toSetDirty);
    size_t size();
};