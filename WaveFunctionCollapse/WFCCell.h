#pragma once
class IWFCManager;
struct WFCPosition;
#include "WFCCellUpdate.h"
#include <optional>

class WFCCell
{
protected:
	IWFCManager* manager;
	WFCPosition* position;
	unsigned long long domain;
	int entropyID;
	int errorID = 0;
public:
	WFCCell(IWFCManager* m, WFCPosition* position, unsigned long long domain);
	WFCCell(WFCCell* other);

	//~WFCCell();
	//WFCCellDomain Domain;
	unsigned long long CollapsedTile;

	// Methods
	void RuleSetup() const;
	void SetDomain(const unsigned long long newDomain);
	const unsigned long long GetDomain();
	unsigned long long CalculateEntropy() const;
	WFCCellUpdate* Collapse();
	WFCCellUpdate* Collapse(unsigned long long toCollapseTo);
	const WFCPosition* GetPosition();
	WFCCellUpdate* DomainCheck(WFCCellUpdate* update);
	int GetError();

	int GetEntropyID();
	void SetEntropyID(int entropyID);

	bool operator<(const WFCCell& other) const
	{
		return (this->CalculateEntropy() < other.CalculateEntropy());
	}

	bool operator>(const WFCCell& other) const {
		return (this->CalculateEntropy() > other.CalculateEntropy());
	}
};

