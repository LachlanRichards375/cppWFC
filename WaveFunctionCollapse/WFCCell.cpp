#pragma once
#include "WFCCell.h"

float WFCCell::CalculateEntropy() const
{
    return 0.0f;
}

WFCCellUpdate& WFCCell::Collapse()
{
    return *new WFCCellUpdate(0,0,this);
}

WFCCellUpdate& WFCCell::Collapse(unsigned long toCollapseTo)
{
    return *new WFCCellUpdate(0,0,this);
}

WFCPosition& WFCCell::GetPosition()
{
    return position;
}

std::optional<WFCCellUpdate> WFCCell::DomainCheck(WFCCellUpdate& update)
{
    std::optional<WFCCellUpdate> returner = std::optional<WFCCellUpdate>();
    return returner;
}
