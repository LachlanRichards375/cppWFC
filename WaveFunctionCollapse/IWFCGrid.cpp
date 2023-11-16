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
    std::shared_ptr <WFCCell> item = entropyQueue.extract(entropyQueue.begin()).value();
    entropyQueue.erase(item);
    entropyQueue.insert(item);
}

WFCPosition& IWFCGrid::GetSize() const
{
    return size;
}


std::shared_ptr<WFCCell> IWFCGrid::PopNextCellToCollapse() {
    return entropyQueue.extract(entropyQueue.begin()).value();
}

int IWFCGrid::RemainingCellsToCollapse()
{
    return entropyQueue.size();
}
