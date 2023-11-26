#include "WFCExternRunner.h"
#include "WFCRuleManager.h"
#include "WFCRuleCellIsNot.h"

extern "C" {

	IWFCCollapseMethod* Threaded2DCollapse_Create() {
		return new IWFCCollapseMethod();
	}

	WFCPosition * WFCPosition_Create2D(unsigned int x, unsigned int y)									{ return new WFCPosition(x, y); }
	WFCPosition * WFCPosition_Create3D(unsigned int x, unsigned int y, unsigned int z)					{ return new WFCPosition(x, y, z); }
	WFCPosition * WFCPosition_Create4D(unsigned int x, unsigned int y, unsigned int z, unsigned int w)	{ return new WFCPosition(x, y, z, w); }
	
	Grid2D* Grid2D_Create(WFCPosition* position) {
		return new Grid2D(*position);
	}

	IWFCManager* IWFCManager_Collapse(IWFCManager* manager, unsigned long toCollapseTo, WFCPosition* position) {
		manager->CollapseSpecificCell(position, toCollapseTo);
		return manager;
	}

	IWFCManager* IWFCManager_Create(IWFCCollapseMethod* collapseMethod, IWFCGrid* grid) {
		return new IWFCManager(collapseMethod, grid);
	}

	void AddTileToDomain(unsigned long tilesToAdd)
	{
		WFCRuleManager::AddToInitialDomain(tilesToAdd);
	}

	void IWFCManager_Run(IWFCManager * manager) {
		manager->Generate();
	}

	void WFCRule_Add_CellIsNot(unsigned long tile, unsigned long goal, unsigned int localTargetCount, WFCPosition localTargets[])
	{
		std::vector<WFCPosition*>localTargetsVec {};
		for (unsigned int i = 0; i < localTargetCount; ++i) {
			localTargetsVec.push_back(new WFCPosition { localTargets[i] });
		}
		WFCRuleManager::AddRuleToTile(tile, new WFCRuleCellIsNot(goal, tile, localTargetsVec));
	}

}