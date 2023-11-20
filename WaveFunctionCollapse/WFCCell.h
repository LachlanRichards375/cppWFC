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
	unsigned long domain;
public:
	WFCCell(IWFCManager* m, WFCPosition* position, unsigned long domain);
	WFCCell(WFCCell* other);

	//~WFCCell();
	//WFCCellDomain Domain;
	unsigned long CollapsedTile;

	// Methods
	void RuleSetup() const;
	void SetDomain(const unsigned long newDomain);
	unsigned long CalculateEntropy() const;
	WFCCellUpdate* Collapse();
	WFCCellUpdate* Collapse(unsigned long toCollapseTo);
	const WFCPosition* GetPosition();
	std::optional<WFCCellUpdate> DomainCheck(WFCCellUpdate* update);

	bool operator<(const WFCCell& other) const
	{
		return (this->CalculateEntropy() < other.CalculateEntropy());
	}

	bool operator>(const WFCCell& other) const {
		return (this->CalculateEntropy() > other.CalculateEntropy());
	}
};

