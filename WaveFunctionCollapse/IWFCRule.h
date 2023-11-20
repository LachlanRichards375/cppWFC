#pragma once
#include"WFCPosition.h"
#include "WFCCellUpdate.h"
#include "IWFCManager.h"

class IWFCRule {
protected:
	std::vector<WFCPosition*> localPositionsWeCareAbout;
	unsigned long goal;
public:
	IWFCRule(unsigned long goal, std::vector<WFCPosition*> localPositionsWeCareAbout);
	virtual bool Test(WFCCellUpdate update, const WFCPosition* currentCellPosition) = 0;
	unsigned long const GetGoal();
	std::vector< WFCPosition*> GetPositions();
};

/*

Each Cell has a list of Tiles (Ulong)
Each tile has a list of rules,
Each rule has a list of local positions it affects

When we test, we need to see if we affect the position that was updated
We pass in the position of the cell
and see if the updated cell exists within our positions we care about


MANAGER INITALIZED:
MANAGER.Generate_Once()
	Grid.Sort()
	CollapseMethod.Collapse()
		Manager.GetAlertees() << vector of pointers
	foreach in alertees,
		test(updateThatOccured, currentCellPos)
*/