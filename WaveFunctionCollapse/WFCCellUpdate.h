#pragma once
class WFCCell;

struct WFCCellUpdate
{
public:
	WFCCellUpdate(unsigned long removedFromDomain, unsigned long addedToDomain, WFCCell* updatedCell);
	unsigned long removedFromDomain;
	unsigned long addedToDomain;
	WFCCell& updatedCell;
};

