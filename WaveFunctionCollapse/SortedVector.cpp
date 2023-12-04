#pragma once
#include "SortedVector.h"


SortedVector::SortedVector()
{
    //Set sortedData to contain 4bytes*8 = 32 (bits) array positions
    sortedData.resize(sizeof(unsigned long)*8);
    vectorSize = 0;
}

void SortedVector::insert(WFCCell*& value)
{
    sortedData[value->CalculateEntropy()].push_back(value);
    ++vectorSize;
}

WFCCell* SortedVector::pop() {
    //Get first list with some elements in it
    std::vector<WFCCell*>* listWorkingOn = &sortedData[0];
    while ((*listWorkingOn).size() <= 0) {
        ++listWorkingOn;
    }
    int randomIndex = rand() % (*listWorkingOn).size();
    WFCCell* item{ (*listWorkingOn)[randomIndex] };
    (*listWorkingOn).erase((*listWorkingOn).begin() + randomIndex);
    --vectorSize;
    return item;
}

WFCCell* SortedVector::popSpecific(WFCPosition* position, int numBits)
{
    for (auto it = sortedData[numBits].begin(); it != sortedData[numBits].end(); ++it) {
        if (*(*it)->GetPosition() == *position) {
            WFCCell* ptr = (*it);
            sortedData[numBits].erase(it);
            --vectorSize;
            return ptr;
        }
    }
    return nullptr;
}

static bool sortPointers(WFCCell* a, WFCCell* b) { return *a < *b; };

void SortedVector::sort() {
    int currentIndex = 0;
    std::vector<int> cellsToRemove;
    for (std::vector<WFCCell*> currentEntropy : sortedData) {
        cellsToRemove = {};
        int cellRemoverCounter = 0;
        for (auto it = currentEntropy.begin(); it != currentEntropy.end(); ++it) {
            int numBitsInDomain = (*it)->CalculateEntropy();
            if (numBitsInDomain!= currentIndex) {
                sortedData[numBitsInDomain].push_back(*it);
                cellsToRemove.push_back(cellRemoverCounter);
            }
            cellRemoverCounter++;
        }
        //Remove cells AFTER we've finished crawling the array
        int cellsRemoved = 0;
        for (int i : cellsToRemove) {
            currentEntropy.erase(currentEntropy.begin() + (i - cellsRemoved));
            ++cellsRemoved;
        }
        ++currentIndex;
    }
}

size_t SortedVector::size() {
    return vectorSize;
}
