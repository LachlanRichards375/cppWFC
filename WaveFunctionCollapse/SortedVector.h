#pragma once
#include <vector>   //std::vector
#include <algorithm> //std::sort
#include "WFCCell.h"
class SortedVector {
private:
    std::vector<WFCCell*> data;

public:
    void insert(WFCCell*& value);
    WFCCell* pop();
    WFCCell* popSpecific(WFCPosition* position);
    void sort();
    size_t size();
};