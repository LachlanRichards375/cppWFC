#pragma once
#include "WFCCell.h"
#include"WFCRuleManager.h"
#include <iostream>
#include "../tracy/public/tracy/Tracy.hpp"

WFCCell::WFCCell(IWFCManager* m, WFCPosition* position, unsigned long domain) : manager(m), position(position)
{
    CollapsedTile = 0;
    WFCCell::domain = domain;
    entropyID = 0;
}

WFCCell::WFCCell(WFCCell* other) : manager(other->manager), position(other->position)
{
    CollapsedTile = other->CollapsedTile;
    domain = other->domain;
    entropyID = other->entropyID;
}

void WFCCell::RuleSetup() const
{
    auto rules = WFCRuleManager::GetRulesForDomain(domain);
    for (auto& rule : rules) {
        for (auto& position : rule->GetPositions()) {
            WFCPosition* globalPos = (*WFCCell::position + *position);
            manager->RegisterForAlert(globalPos, const_cast<WFCCell*>(this));
        }
    }
}

WFCCellUpdate* WFCCell::Collapse()
{
    //std::cout << "(Collapse() does not do tile weighting) ";
    int bitNumToUse{ rand() % (WFCRuleManager::GetBitsInDomain(domain)) };
    int index = 0;
    int flippedBitsFound = 0;
    while(flippedBitsFound <= bitNumToUse){
        if ((1 << index & domain) != 0) {
            ++flippedBitsFound;
        }
        ++index;
    }
    return Collapse(1 << index - 1);
}

WFCCellUpdate* WFCCell::Collapse(unsigned long toCollapseTo)
{
    CollapsedTile = toCollapseTo;
    #ifdef _DEBUG
        std::cout << "Collapsing cell (" << position->x << "," << position->y << ") to " << CollapsedTile << std::endl;
    #endif
    return new WFCCellUpdate(domain & ~toCollapseTo,0,toCollapseTo,position);
}


WFCCellUpdate* WFCCell::DomainCheck(WFCCellUpdate* update)
{
    ZoneScopedN("DomainCheck in Cell");

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
   
    if (!returnUpdate) {
        return nullptr;
    }

    //Before we update we need to notify sorted queue we want to mark cell as dirty
    //manager->MarkDirty(this);

    domain &= ~updateToReturn->removedFromDomain;
    
    
    #ifdef _DEBUG
        std::cout << "(" << position->x << "," << position->y << ") has domain " << domain << " after domain check " << std::endl;
    #endif // DEBUG

    //only include bits not flipped in removed from domain
    if (domain == 0 && CollapsedTile == 0) {
        std::cout << "\n\nERROR: Domain = 0 on cell " << position->x << "," << position->y << std::endl;
       errorID = - 2;
       return nullptr;
    }
    return updateToReturn;
}

int WFCCell::GetError()
{
    return errorID;
}

int WFCCell::GetEntropyID() {
    return entropyID;
}

void WFCCell::SetEntropyID(int entropyID)
{
    WFCCell::entropyID = entropyID;
}

const WFCPosition* WFCCell::GetPosition()
{
    return position;
}

void WFCCell::SetDomain(const unsigned long newDomain)
{
    domain = newDomain;
}

const unsigned long WFCCell::GetDomain() { 
    if (CollapsedTile > 0) {
        return 0;
    } else {
        return domain;
    }
}

unsigned long WFCCell::CalculateEntropy() const
{

    if (CollapsedTile > 0) {
        return 0;
    } else {
        //We want to return the number of bits in domain otherwise it will favour smaller ids
        return WFCRuleManager::GetBitsInDomain(domain);
    }
}