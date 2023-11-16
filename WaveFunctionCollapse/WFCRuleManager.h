#pragma once
#include "WFCPosition.h"
#include "IWFCRule.h"
#include <vector>

class WFCRuleManager
{
private:
	//Preallocate an array for the rule vectors
	static std::vector<std::shared_ptr<IWFCRule>> rulesOnTiles[sizeof(unsigned long)];
public:
	//static std::vector<WFCPosition&> GetRulesAffectingDomain(unsigned long domain);
	static void AddRuleToTile(unsigned long ruleAppliesTo, std::shared_ptr<IWFCRule> rule);
	static std::vector<std::shared_ptr<IWFCRule>> GetRulesForTile(unsigned long tiles);
};