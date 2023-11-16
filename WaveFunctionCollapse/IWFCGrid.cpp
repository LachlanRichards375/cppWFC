#include "IWFCGrid.h"
#include "WFCCell.h"

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
    WFCCell* item = entropyQueue.extract(entropyQueue.begin()).value();
    entropyQueue.erase(item);
    entropyQueue.insert(item);
}

WFCPosition& IWFCGrid::GetSize() const
{
    return size;
}

WFCCell* IWFCGrid::PopNextCellToCollapse() {
    return entropyQueue.extract(entropyQueue.begin()).value();
}

int IWFCGrid::RemainingCellsToCollapse()
{
    return entropyQueue.size();
}
