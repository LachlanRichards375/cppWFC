#pragma once
#include "WFCPosition.h"
#include "IWFCRule.h"
#include <vector>

class WFCRuleManager
{
private:
	//Preallocate an array for the rule vectors
	static std::vector<std::vector<IWFCRule*>> rulesOnTiles;
	static int getLeastSignifigantBit(unsigned long long);
	static unsigned long long activeDomain;
public:
	//static std::vector<WFCPosition&> GetRulesAffectingDomain(unsigned long long domain);
	static void AddRuleToTile(unsigned long long ruleAppliesTo, IWFCRule* rule);
	static std::vector<IWFCRule*> GetRulesForTile(unsigned long long tiles);
	static std::vector<IWFCRule*> GetRulesForDomain(unsigned long long domain);
	static unsigned long long GetInitialDomain();
	static void AddToInitialDomain(unsigned long long domain);
	static int GetBitsInDomain(unsigned long long domain);
};