#pragma once
#include "WFCPosition.h"

struct WFCCellUpdate
{
public:
	WFCCellUpdate(unsigned long removed, unsigned long added, unsigned long collapsed, const WFCPosition* position);
	unsigned long removedFromDomain;
	unsigned long addedToDomain;
	unsigned long collapsedTo;
	const WFCPosition* updatedCell;
};

