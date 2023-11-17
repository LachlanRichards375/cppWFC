#pragma once
#include "WFCPosition.h"
#include "IWFCRule.h"
#include <vector>

class WFCRuleManager
{
private:
	//Preallocate an array for the rule vectors
	static std::vector<std::vector<std::shared_ptr<IWFCRule>>> rulesOnTiles;
	static int getLeastSignifigantBit(unsigned long);
	static unsigned long activeDomain;
public:
	//static std::vector<WFCPosition&> GetRulesAffectingDomain(unsigned long domain);
	static void AddRuleToTile(unsigned long ruleAppliesTo, std::shared_ptr<IWFCRule> rule);
	static std::vector<std::shared_ptr<IWFCRule>> GetRulesForTile(unsigned long tiles);
	static unsigned long GetInitialDomain();
	static void AddToInitialDomain(unsigned long domain);
	static int GetBitsInDomain(unsigned long domain);
};