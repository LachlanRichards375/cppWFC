#pragma once
#include "WFCRuleManager.h"
#include <math.h>
#include <iostream>

std::vector<std::vector<IWFCRule*>> WFCRuleManager::rulesOnTiles;
const int RuleBufferLength = 32;
//RuleNode WFCRuleManager::ruleBuffer[RuleBufferLength];
std::vector<RuleNode> WFCRuleManager::ruleBuffer{};

std::mutex WFCRuleManager::ruleBufferMutex;

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
	ZoneScopedN("GetRules");
	int ruleBufferLastIndex = 0;
	if (ruleBuffer.size() != RuleBufferLength) {
		ruleBuffer.resize(RuleBufferLength);
	}

	for (int i = 0; i < RuleBufferLength; i++) {
		ruleBufferLastIndex = i;
		if (ruleBuffer[i].tileAppliedTo == 0) {
			break;
		}

		if (ruleBuffer[i].tileAppliedTo == domain) {
			//swap current buffer up one if possible
			if (i > 0) {
				std::lock_guard<std::mutex> lock(ruleBufferMutex);
				std::swap(ruleBuffer[i], ruleBuffer[i - 1]);
				return ruleBuffer[i].rules;
			}
		}
	}
	std::vector<IWFCRule*> rulesToReturn{};
	const unsigned long long One = 1;
	for (int i = 0; i < rulesOnTiles.size(); ++i) {
		ZoneScopedN("for each rule list");
		if ((One << i) & domain) {
			for (IWFCRule* ruleToApply : rulesOnTiles[i]) {
				ZoneScopedN("pushBack rules To Return");
				rulesToReturn.push_back(ruleToApply);
			}
		}
	}
	// Replace the last element in the RuleBuffer array with this ruleset
	{

		std::lock_guard<std::mutex> lock(ruleBufferMutex);
		ruleBuffer[ruleBufferLastIndex].rules = { rulesToReturn };
		ruleBuffer[ruleBufferLastIndex].tileAppliedTo = domain;
	}
	return rulesToReturn;
}
