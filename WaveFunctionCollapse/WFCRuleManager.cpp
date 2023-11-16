#include "WFCRuleManager.h"
#include <math.h>
//PreAllocate the rulesOnTiles
//std::vector<std::shared_ptr<IWFCRule>> WFCRuleManager::rulesOnTiles[sizeof(unsigned long)];
std::vector<std::vector<std::shared_ptr<IWFCRule>>> WFCRuleManager::rulesOnTiles;
unsigned long WFCRuleManager::activeDomain{0};

void WFCRuleManager::AddToInitialDomain(unsigned long domain) {
	WFCRuleManager::activeDomain |= domain;
}

unsigned long WFCRuleManager::GetInitialDomain() {
	return activeDomain;
}

void WFCRuleManager::AddRuleToTile(unsigned long ruleAppliesTo, std::shared_ptr<IWFCRule> rule)
{
	if (rulesOnTiles.size() == 0) {
		rulesOnTiles.resize(sizeof(unsigned long));
	}

	auto& lookup {rulesOnTiles[getLeastSignifigantBit(ruleAppliesTo) + 1]};

	if (std::find(lookup.begin(), lookup.end(), rule) != lookup.end()) {
		return;
	}
	lookup.push_back(rule);
}

int WFCRuleManager::getLeastSignifigantBit(unsigned long ulong) {
	int index{ 0 };
	while ((ulong & 1) == 0) {
		ulong >>= 1;
		index++;
	}
	return index;
}

std::vector<std::shared_ptr<IWFCRule>> WFCRuleManager::GetRulesForTile(unsigned long tile)
{
	if (tile == 0) {
		return rulesOnTiles.at(0);
	}

	 // Add 1 to get the 1-based index
	return rulesOnTiles[getLeastSignifigantBit(tile)+1];
}
