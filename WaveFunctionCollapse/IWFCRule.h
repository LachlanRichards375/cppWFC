#pragma once
#include"WFCPosition.h"
#include "WFCCellUpdate.h"
#include "IWFCManager.h"

class IWFCRule {
public:
	virtual bool Test(unsigned long ownerTile, WFCPosition& localTargetOffset, WFCCellUpdate update) = 0;
	virtual void Initialize(IWFCManager& manager, WFCPosition& cellPosition) = 0;
};
