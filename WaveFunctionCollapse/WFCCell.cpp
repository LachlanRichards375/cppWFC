#pragma once
#include "WFCCell.h"
#include"WFCRuleManager.h"
#include <iostream>

WFCCell::WFCCell(IWFCManager* m, WFCPosition* position, unsigned long domain) : manager(m), position(position)
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
        for (auto& position : rule->GetPositions()) {
            WFCPosition* globalPos = new WFCPosition{ *position - WFCCell::position };
            manager->RegisterForAlert(globalPos, const_cast<WFCCell*>(this));
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

WFCCellUpdate* WFCCell::Collapse()
{
    //std::cout << "(Collapse() does not do tile weighting) ";
    int index{ rand() % (WFCRuleManager::GetBitsInDomain(domain)) };
    return Collapse((unsigned long)1 << index);
}

WFCCellUpdate* WFCCell::Collapse(unsigned long toCollapseTo)
{
    CollapsedTile = toCollapseTo;
    std::cout << "Collapsing cell (" << position->x << "," << position->y << ") to " << CollapsedTile << std::endl;
    return new WFCCellUpdate(domain & ~toCollapseTo,0,toCollapseTo,position);
}

const WFCPosition* WFCCell::GetPosition()
{
    return position;
}

std::optional<WFCCellUpdate> WFCCell::DomainCheck(WFCCellUpdate* update)
{
    WFCCellUpdate updateToReturn = WFCCellUpdate(0, 0, 0, position);
    std::vector<IWFCRule*> rulesList = WFCRuleManager::GetRulesForDomain(domain);
    bool returnUpdate = false;
    for (auto& rule : rulesList) {
        if (!rule->Test(*update, position)) {
            //Remove this tile from the domain
            updateToReturn.removedFromDomain |= rule->GetTile();
            domain &= ~rule->GetTile();
            returnUpdate = true;
        }
    }
    //only include bits not flipped in removed from domain
    domain &= ~updateToReturn.removedFromDomain;
    if (returnUpdate) {
        std::optional<WFCCellUpdate> returner(*update);
        return returner;
    }
    else {
        return std::optional<WFCCellUpdate>();
    }
}
