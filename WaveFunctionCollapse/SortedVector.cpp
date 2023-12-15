#pragma once
#include "SortedVector.h"
//#include "../tracy/public/tracy/Tracy.hpp"
#include <iostream>


SortedVector::SortedVector()
{
    //Set sortedData to contain 4bytes*8 = 32 (bits) array positions
    sortedData.resize(sizeof(unsigned long) * 8);
    dirtyData.resize(sizeof(unsigned long) * 8);
    vectorSize = 0;
}

void SortedVector::insert(WFCCell* value)
{
    //ZoneScopedN("insert");
    int bitsInDomain = value->CalculateEntropy();
    sortedData[bitsInDomain].push_back(value);
    value->SetEntropyID(sortedData[bitsInDomain].size()-1);
    ++vectorSize;
}

WFCCell* SortedVector::pop() {
   // ZoneScopedN("SortedQueue pop");
    //Get first list with some elements in it
    int listIndex = 0;
    std::vector<WFCCell*>* listWorkingOn = &sortedData[0];
    while ((*listWorkingOn).size() <= 0) {
        ++listWorkingOn;
        ++listIndex;
    }
    int randomIndex = rand() % (*listWorkingOn).size();
    WFCCell* item{ (*listWorkingOn)[randomIndex] };
    (*listWorkingOn).erase((*listWorkingOn).begin() + randomIndex);
    ResetEntropyID(listIndex, randomIndex);
    --vectorSize;
    return item;
}

WFCCell* SortedVector::popSpecific(WFCPosition* position, int numBits)
{
    //ZoneScopedN("SortedQueue popSpecific");
    int index = 0;
    for (auto it = sortedData[numBits].begin(); it != sortedData[numBits].end(); ++it) {
        if (*(*it)->GetPosition() == *position) {
            WFCCell* ptr = (*it);
            sortedData[numBits].erase(it);
            //Reset the entropy id in the current bitMask
            ResetEntropyID(numBits, index);
            --vectorSize;
            return ptr;
        }
        ++index;
    }
    return nullptr;
}

void SortedVector::sort() {
    //ZoneScopedN("sort");
    //For each bit
        //See if it has anything in dirty data
            //if 0 skip
        
        //For each index in dirty data
            //Go to that index - the number we've removed
            //re-insert it to correct the position
            //remove the old copy

        //Update all the old EntropyID within the vector we just updated

    for (int NumOfBits = 1; NumOfBits < sortedData.size(); ++NumOfBits) {
        //If there are dirty cells to clean
        if (dirtyData[NumOfBits].size() == 0) {
            continue;
        }

        std::vector<WFCCell*> toReinsert{};

        auto sortedIt = sortedData[NumOfBits].begin();
        int numToRemoveAtStart = dirtyData[NumOfBits].size();
        //remove all the dirty values
        for (int numberRemovedFromSorted = 0; numberRemovedFromSorted < numToRemoveAtStart; ++numberRemovedFromSorted) {
            //We need to re-validate the iterator
            sortedIt = sortedData[NumOfBits].begin();
            std::advance(sortedIt, dirtyData[NumOfBits][numberRemovedFromSorted] - numberRemovedFromSorted);

            toReinsert.push_back(*sortedIt);
            sortedData[NumOfBits].erase(sortedIt);
        }

        //Reset the entropy id in the current bitMask
        ResetEntropyID(NumOfBits, dirtyData[NumOfBits][0]);

        //Add 'dirty' values back into the queue to clean them
        for (auto toAdd : toReinsert) {
            insert(toAdd);
            //Need to reduce vectorSize as insert will add it
            --vectorSize;
        }

        //clear the dirty bitmask
        dirtyData[NumOfBits].clear();
    }
}

void SortedVector::ResetEntropyID(int bitNumToReset, int numToStartAt = 0) {
    for (int i = numToStartAt; i < sortedData[bitNumToReset].size(); i++) {
        //reset all entropy id's after this position
        sortedData[bitNumToReset][i]->SetEntropyID(i);
    }
}

void SortedVector::SetDirty(unsigned long oldDomainCount, int index) {
    for (int check : dirtyData[oldDomainCount]) {
        if (check == index) {
            return;
        }
    }
    dirtyData[oldDomainCount].emplace_back(index);
}

size_t SortedVector::size() {
    return vectorSize;
}

void SortedVector::clear()
{
    sortedData.resize(sizeof(unsigned long) * 8);
    dirtyData.resize(sizeof(unsigned long) * 8);
    vectorSize = 0;
}
