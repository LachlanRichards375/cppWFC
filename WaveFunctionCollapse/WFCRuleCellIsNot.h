#pragma once
#include "IWFCRule.h"
class WFCRuleCellIsNot :
    public IWFCRule
{
public:
    WFCRuleCellIsNot(unsigned long goal, std::vector< std::shared_ptr<WFCPosition>> localPositionsWeCareAbout);
    virtual bool Test(WFCCellUpdate update, const WFCPosition& currentCellPosition) override;
};

