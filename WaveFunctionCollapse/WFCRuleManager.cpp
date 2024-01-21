#pragma once
#include "WFCRuleManager.h"
#include <math.h>
#include <iostream>
//PreAllocate the rulesOnTiles
//std::vector<std::shared_ptr<IWFCRule>> WFCRuleManager::rulesOnTiles[sizeof(unsigned long long)];
std::vector<std::vector<IWFCRule*>> WFCRuleManager::rulesOnTiles;
unsigned long long WFCRuleManager::activeDomain{0};

void WFCRuleManager::AddToInitialDomain(unsigned long long domain) {
	WFCRuleManager::activeDomain |= domain;
}

unsigned long long WFCRuleManager::GetInitialDomain() {
	return activeDomain;
}

int WFCRuleManager::GetBitsInDomain(unsigned long long domain)
{
	int numBits{ 0 };
	int index = getLeastSignifigantBit(domain);

	while (domain > 0 ) {
		//Bitshift 1 left index bits, compare against domain
		if ((1 & domain) > 0) {
			numBits++;
		}
		domain >>= 1;
	}

	return numBits;
}

void WFCRuleManager::AddRuleToTile(unsigned long long ruleAppliesTo, IWFCRule* rule)
{
	if (rulesOnTiles.size() == 0) {
		rulesOnTiles.resize(sizeof(unsigned long long) * 8); //sizeof returns bytes
	}

	auto& lookup {rulesOnTiles[getLeastSignifigantBit(ruleAppliesTo)]};

	if (std::find(lookup.begin(), lookup.end(), rule) != lookup.end()) {
		return;
	}
	std::cout << "Adding rule: " << rule->to_string();
	lookup.push_back(rule);
}

int WFCRuleManager::getLeastSignifigantBit(unsigned long long ulong) {
	int index{ 0 };
	while ((ulong & 1) == 0) {
		ulong >>= 1;
		index++;
	}
	return index;
}

std::vector<IWFCRule*> WFCRuleManager::GetRulesForTile(unsigned long long tile)
{
	if (tile == 0) {
		return rulesOnTiles.at(0);
	}

	// Add 1 to get the 1-based index
	return rulesOnTiles[getLeastSignifigantBit(tile)];
}

std::vector<IWFCRule*> WFCRuleManager::GetRulesForDomain(unsigned long long domain)
{
	std::vector<IWFCRule*> rulesToReturn{};
	for (int i = 0; i < rulesOnTiles.size(); ++i) {
		if ((static_cast<unsigned long long>(1) << i & domain) > 0) {
			for (IWFCRule* ruleToApply : rulesOnTiles[i]) {
				rulesToReturn.push_back(ruleToApply);
			}
		}
	}
	// Add 1 to get the 1-based index
	return rulesToReturn;
}
