#pragma once
#include <vector>
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