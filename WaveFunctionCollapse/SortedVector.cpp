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
