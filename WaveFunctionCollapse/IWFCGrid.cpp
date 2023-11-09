#include "IWFCGrid.h"

IWFCGrid::IWFCGrid(WFCPosition& newSize) : size(newSize)
{
}

WFCPosition& IWFCGrid::GetSize()
{
    return size;
}
