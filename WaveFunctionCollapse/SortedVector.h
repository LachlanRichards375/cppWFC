#pragma once
#include <vector>
#include "WFCCell.h"
class SortedVector {
private:
    std::vector<WFCCell*> data;

public:
    void insert(WFCCell*& value);
    WFCCell* pop();
    void sort();
    size_t size();
};

//class SortedWFCCellList {
//    private:
//    std::vector<T> data;
//
//public:
//    void insert(const T& value);
//    T pop();
//    void sort();
//    size_t size();
//};
//};