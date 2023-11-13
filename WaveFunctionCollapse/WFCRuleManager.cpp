#include "WFCRuleManager.h"

//PreAllocate the rulesOnTiles
std::vector<std::shared_ptr<IWFCRule>> WFCRuleManager::rulesOnTiles[sizeof(unsigned long)];

void WFCRuleManager::AddRuleToTile(unsigned long ruleAppliesTo, std::shared_ptr<IWFCRule> rule)
{
	auto& lookup {rulesOnTiles[ruleAppliesTo]};

	if (std::find(lookup.begin(), lookup.end(), rule) == lookup.end()) {
		return;
	}
	lookup.push_back(rule);
}