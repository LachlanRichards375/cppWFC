#pragma once
#include "SortedVector.h"


void SortedVector::insert(WFCCell*& value)
{
    auto it = std::lower_bound(data.begin(), data.end(), value);
    data.insert(it, value);
}

WFCCell* SortedVector::pop() {
    WFCCell* item{ data[0] };
    data.erase(data.begin());
    return item;
}

WFCCell* SortedVector::popSpecific(WFCPosition* position)
{
    for (auto it = data.begin(); it != data.end(); ++it) {
        if (*(*it)->GetPosition() == *position) {
            WFCCell* ptr = (*it);
            data.erase(it);
            return ptr;
        }
    }
    return nullptr;
}

static bool sortPointers(WFCCell* a, WFCCell* b) { return *a < *b; };

void SortedVector::sort() {
    std::sort(data.begin(), data.end(), sortPointers);
}

size_t SortedVector::size() {
    return data.size();
}
