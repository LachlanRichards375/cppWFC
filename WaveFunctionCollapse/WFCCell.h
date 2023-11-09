#pragma once
class IWFCManager;
struct WFCPosition;
#include "WFCCellUpdate.h"
#include <optional>

class WFCCell
{
protected:
	IWFCManager& manager;
	WFCPosition& position;
	unsigned long domain;
public:
	WFCCell(IWFCManager &m, WFCPosition &p, const unsigned long domain) : manager(m), position(p)
	{
		CollapsedTile = 0;
		WFCCell::domain = domain;
	}
	WFCCell(WFCCell& other) : manager(other.manager), position(other.position)
	{
		CollapsedTile = other.CollapsedTile;
		domain = other.domain;
	}

	//~WFCCell();
	//WFCCellDomain Domain;
	unsigned long CollapsedTile;

	// Methods
	//void RuleSetup() const;
	void SetDomain(const unsigned long newDomain);
	float CalculateEntropy() const;
	WFCCellUpdate& Collapse();
	WFCCellUpdate& Collapse(unsigned long toCollapseTo);
	WFCPosition& GetPosition();
	std::optional<WFCCellUpdate> DomainCheck(WFCCellUpdate& update);

	bool operator<(const WFCCell& other) const
	{
		return (this->CalculateEntropy() < other.CalculateEntropy());
	}

	bool operator>(const WFCCell& other) const {
		return (this->CalculateEntropy() > other.CalculateEntropy());
	}
};

