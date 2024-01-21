#pragma once
#include "IWFCRule.h"
class WFCRuleCellIsNot :
    public IWFCRule
{
public:
    WFCRuleCellIsNot(unsigned long long goal, unsigned long long tile, std::vector<WFCPosition*> localPositionsWeCareAbout);
    virtual bool Test(WFCCellUpdate update, const WFCPosition* currentCellPosition) override;
    virtual std::string to_string() override;
};

