#include "IWFCGrid.h"
#include "WFCCell.h"
#include "SortedVector.h"

IWFCGrid::IWFCGrid(WFCPosition& newSize) : size(newSize)
{
    manager = nullptr;
}

void IWFCGrid::SetManager(IWFCManager* manager)
{
    IWFCGrid::manager = manager;
}

void IWFCGrid::SortQueue()
{
    //push sorts queue when executed
    entropyQueue.sort();
}

WFCPosition& IWFCGrid::GetSize() const
{
    return size;
}

WFCCell* IWFCGrid::PopNextCellToCollapse() {
    return entropyQueue.pop();
}

size_t IWFCGrid::RemainingCellsToCollapse()
{
    return entropyQueue.size();
}

