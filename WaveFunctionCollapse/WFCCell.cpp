#pragma once
#include "WFCCell.h"
#include"WFCRuleManager.h"

void WFCCell::RuleSetup() const
{
}

void WFCCell::SetDomain(const unsigned long newDomain)
{
    domain = newDomain;
}

float WFCCell::CalculateEntropy() const
{
    return 0.0f;
}

WFCCellUpdate& WFCCell::Collapse()
{
    return *new WFCCellUpdate(0,0,0,position);
}

WFCCellUpdate& WFCCell::Collapse(unsigned long toCollapseTo)
{
    return *new WFCCellUpdate(0,0,0,position);
}

const WFCPosition& WFCCell::GetPosition()
{
    return position;
}

std::optional<WFCCellUpdate> WFCCell::DomainCheck(WFCCellUpdate& update)
{
    WFCCellUpdate updateToReturn = WFCCellUpdate(0, 0, 0, position);
    std::vector<std::shared_ptr<IWFCRule>> rulesList = WFCRuleManager::GetRulesForTile(domain);
    for (std::shared_ptr<IWFCRule> rule : rulesList) {
        if (!rule->Test(update, position)) {
            //Remove this tile from the domain
            updateToReturn.removedFromDomain = updateToReturn.removedFromDomain|rule->GetGoal();
        }
    }
    std::optional<WFCCellUpdate> returner (update);
    return returner;
}
