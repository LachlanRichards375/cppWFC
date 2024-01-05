#pragma once
#include "WFCPosition.h"

struct WFCCellUpdate
{
public:
	WFCCellUpdate(unsigned long long removed, unsigned long long added, unsigned long long collapsed, const WFCPosition* position);
	unsigned long long removedFromDomain;
	unsigned long long addedToDomain;
	unsigned long long collapsedTo;
	const WFCPosition* updatedCell;
};

