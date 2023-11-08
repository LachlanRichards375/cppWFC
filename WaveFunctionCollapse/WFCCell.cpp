#pragma once
#include "WFCCell.h"

float WFCCell::CalculateEntropy() const
{
    return 0.0f;
}

WFCCellUpdate WFCCell::Collapse()
{
    return WFCCellUpdate();
}

WFCCellUpdate WFCCell::Collapse(unsigned long toCollapseTo)
{
    return WFCCellUpdate();
}
