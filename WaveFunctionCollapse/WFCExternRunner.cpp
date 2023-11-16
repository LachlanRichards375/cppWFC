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
	
	IWFCManager* IWFCManager_Create(IWFCCollapseMethod* collapseMethod, IWFCGrid* grid) {
		return new IWFCManager(collapseMethod, grid);
	}

	void IWFCManager_Run(IWFCManager * manager) {
		manager->Generate();
	}

	void WFCRule_Add_CellIsNot(unsigned long tile, unsigned long goal, WFCPosition * localTargets[])
	{
		std::vector<std::shared_ptr<WFCPosition>>localTargetsVec {};
		int len = sizeof localTargets / sizeof localTargets[0];
		for (int i = 0; i < len; ++i) {
			localTargetsVec.push_back(std::make_shared<WFCPosition>(localTargets[i]));
		}
		WFCRuleManager::AddRuleToTile(tile, std::make_shared<WFCRuleCellIsNot>(goal, localTargetsVec));
	}

}