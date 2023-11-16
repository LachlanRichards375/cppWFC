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

void SortedVector::sort() {
    std::vector<WFCCell*> other{data};
    data.clear();
    std::vector<WFCCell*>::iterator it{other.begin()};
    for (size_t i = other.size(); i > 0; --i) {
        insert(*it);
        it += 1;
    }
}

size_t SortedVector::size() {
    return data.size();
}
