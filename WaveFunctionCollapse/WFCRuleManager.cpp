#include "WFCRuleManager.h"
#include <math.h>
//PreAllocate the rulesOnTiles
std::vector<std::shared_ptr<IWFCRule>> WFCRuleManager::rulesOnTiles[sizeof(unsigned long)];

void WFCRuleManager::AddRuleToTile(unsigned long ruleAppliesTo, std::shared_ptr<IWFCRule> rule)
{
	auto& lookup {rulesOnTiles[ruleAppliesTo]};

	if (std::find(lookup.begin(), lookup.end(), rule) != lookup.end()) {
		return;
	}
	lookup.push_back(rule);
}

std::vector<std::shared_ptr<IWFCRule>> WFCRuleManager::GetRulesForTile(unsigned long tile)
{
	if (tile == 0) {
		return rulesOnTiles[0];
	}
	int index{ 0 };
	while ((tile & 1) == 0) {
		tile >>= 1;
		index++;
	}
	index++; // Add 1 to get the 1-based index
	return rulesOnTiles[index];
}
