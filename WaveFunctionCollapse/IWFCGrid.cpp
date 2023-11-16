#include "IWFCGrid.h"

IWFCGrid::IWFCGrid(WFCPosition& newSize) : size(newSize)
{
}

void IWFCGrid::SetManager(IWFCManager* manager)
{
    IWFCGrid::manager = manager;
}

WFCPosition& IWFCGrid::GetSize()
{
    return size;
}
