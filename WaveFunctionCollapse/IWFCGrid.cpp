#include "IWFCGrid.h"
#include "WFCCell.h"
#include "SortedVector.h"
#include "../tracy/public/tracy/Tracy.hpp"

IWFCGrid::IWFCGrid(WFCPosition& newSize) : size(newSize)
{
    manager = nullptr;
}

void IWFCGrid::SortQueue()
{
    //push sorts queue when executed
    ZoneScopedN("Sorting EntropyQueue");
    entropyQueue.sort();
}

WFCPosition& IWFCGrid::GetSize() const
{
    return size;
}

WFCCell* IWFCGrid::PopNextCellToCollapse() {
    return entropyQueue.pop();
}

WFCCell* IWFCGrid::PopSpecificCell(WFCPosition* position)
{
    return entropyQueue.popSpecific(position);
}

size_t IWFCGrid::RemainingCellsToCollapse()
{
    return entropyQueue.size();
}

