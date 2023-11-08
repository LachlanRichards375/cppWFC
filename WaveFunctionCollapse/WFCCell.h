#pragma once
class IWFCManager;
struct WFCPosition;
#include "WFCCellUpdate.h"

class WFCCell
{
protected:
	IWFCManager &manager;
	WFCPosition &position;
	unsigned long domain;
public:
	WFCCell(IWFCManager &m, WFCPosition &p) : manager(m), position(p)
	{
		CollapsedTile = 0;
		domain = 0;
	}
	WFCCell(WFCCell& other) : manager(other.manager), position(other.position)
	{
		CollapsedTile = 0;
		domain = 0;
	}

	//~WFCCell();
	//WFCCellDomain Domain;
	unsigned long CollapsedTile;
	// event Action<WFCCellUpdate> OnCellUpdate;

	// Methods
	//void SetDomain(const std::vector<unsigned long> newDomain);
	//void RuleSetup() const;
	float CalculateEntropy() const;
	WFCCellUpdate Collapse();
	WFCCellUpdate Collapse(unsigned long toCollapseTo);
	// WFCCellUpdate? DomainCheck(WFCCellUpdate update);
	//WFCPosition GetPosition() const;

	bool operator<(const WFCCell& other) const
	{
		return (this->CalculateEntropy() < other.CalculateEntropy());
	}

	bool operator>(const WFCCell& other) const {
		return (this->CalculateEntropy() > other.CalculateEntropy());
	}
};

