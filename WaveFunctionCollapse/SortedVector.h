#pragma once
#include <vector>
template <typename T>
class SortedVector {
private:
    std::vector<T> data;

public:
    void insert(const T& value);
    T pop();
    void sort();
    size_t size();
};