#pragma once
#include "WFCCell.h"
#include"WFCRuleManager.h"
#include <iostream>

WFCCell::WFCCell(IWFCManager& m, const WFCPosition* p, unsigned long domain) : manager(m), position(p)
{
    CollapsedTile = 0;
    WFCCell::domain = domain;
}

WFCCell::WFCCell(WFCCell* other) : manager(other->manager), position(other->position)
{
    CollapsedTile = other->CollapsedTile;
    domain = other->domain;
}

void WFCCell::RuleSetup() const
{
    auto rules = WFCRuleManager::GetRulesForTile(domain);
    for (auto& rule : rules) {
        for (auto& position : rule.get()->GetPositions()) {
            manager.RegisterForAlert(position, const_cast<WFCCell*>(this));
        }
    }
}

void WFCCell::SetDomain(const unsigned long newDomain)
{
    domain = newDomain;
}

unsigned long WFCCell::CalculateEntropy() const
{
    return domain;
}

WFCCellUpdate& WFCCell::Collapse()
{
    //std::cout << "(Collapse() does not do tile weighting) ";
    int index{ rand() % (WFCRuleManager::GetBitsInDomain(domain)) };
    return Collapse((unsigned long)1 << index);
}

WFCCellUpdate& WFCCell::Collapse(unsigned long toCollapseTo)
{
    //std::cout << "Collapsing cell (" << position->x << "," << position->y << ") to " << toCollapseTo << std::endl;
    return *new WFCCellUpdate(0,0,0,*position);
}

const WFCPosition* WFCCell::GetPosition()
{
    return position;
}

std::optional<WFCCellUpdate> WFCCell::DomainCheck(WFCCellUpdate& update)
{
    WFCCellUpdate updateToReturn = WFCCellUpdate(0, 0, 0, *position);
    std::vector<std::shared_ptr<IWFCRule>> rulesList = WFCRuleManager::GetRulesForTile(domain);
    for (std::shared_ptr<IWFCRule> rule : rulesList) {
        if (!rule->Test(update, *position)) {
            //Remove this tile from the domain
            updateToReturn.removedFromDomain = updateToReturn.removedFromDomain|rule->GetGoal();
        }
    }
    std::optional<WFCCellUpdate> returner (update);
    return returner;
}
