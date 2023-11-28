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

WFCCellUpdate* WFCCell::DomainCheck(WFCCellUpdate* update)
{
    if (CollapsedTile > 0) {
        return nullptr;
    }

    WFCCellUpdate* updateToReturn = new WFCCellUpdate(0, 0, 0, position);
    std::vector<IWFCRule*> rulesList = WFCRuleManager::GetRulesForDomain(domain);
    bool returnUpdate = false;
    for (auto& rule : rulesList) {
        if (!rule->Test(*update, position)) {
            //Remove this tile from the domain
            updateToReturn->removedFromDomain |= rule->GetTile();
            returnUpdate = true;
        }
    }
    //only include bits not flipped in removed from domain
    domain &= ~updateToReturn->removedFromDomain;
    std::cout << "(" << position->x << "," << position->y << ") has domain " << domain << " after domain check " << std::endl;
    if (domain == 0 && CollapsedTile == 0) {
        std::cout << "\n\nERROR: Domain = 0 on cell " << position->x << "," << position->y << std::endl;
        throw -2;
        return nullptr;
    }

    if (returnUpdate) {
        return updateToReturn;
    }
    else {
        return nullptr;
    }
}
