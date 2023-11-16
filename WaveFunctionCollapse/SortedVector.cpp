#include "SortedVector.h"

/*template<typename T>
void SortedVector<T>::insert(const T& value) {
}*/

template<typename T>
void SortedVector<T>::insert(const T& value)
{
    auto it = std::lower_bound(data.begin(), data.end(), value);
    data.insert(it, value);
}

template<typename T>
T SortedVector<T>::pop() {
    T item{ data[0] };
    data.erase(data.front());
    return item;
}

template<typename T>
void SortedVector<T>::sort() {
    std::vector<T> other{data};
    data.clear();
    while (other.size > 0) {
        insert(other.front());
    }
}

template<typename T>
size_t SortedVector<T>::size() {
    return data.size();
}
