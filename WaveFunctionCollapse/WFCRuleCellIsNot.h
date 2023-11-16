#pragma once
#include "IWFCRule.h"
class WFCRuleCellIsNot :
    public IWFCRule
{
public:
    virtual bool Test(WFCCellUpdate update, const WFCPosition& currentCellPosition) override;
};

