#include "WFCExternRunner.h"
#include "WFCRuleManager.h"
#include "WFCRuleCellIsNot.h"

extern "C"  {

	_declspec(dllexport) IWFCCollapseMethod* Threaded2DCollapse_Create() {
		return new IWFCCollapseMethod();
	}

	_declspec(dllexport) WFCPosition * WFCPosition_Create2D(unsigned int x, unsigned int y)									{ return new WFCPosition(x, y); }
	_declspec(dllexport) WFCPosition * WFCPosition_Create3D(unsigned int x, unsigned int y, unsigned int z)					{ return new WFCPosition(x, y, z); }
	_declspec(dllexport) WFCPosition * WFCPosition_Create4D(unsigned int x, unsigned int y, unsigned int z, unsigned int w)	{ return new WFCPosition(x, y, z, w); }
	
	_declspec(dllexport) Grid2D* Grid2D_Create(WFCPosition* position) {
		return new Grid2D(*position);
	}

	_declspec(dllexport) IWFCManager* IWFCManager_Create(IWFCCollapseMethod* collapseMethod, IWFCGrid* grid, short threadCount) {
		return new IWFCManager(collapseMethod, grid, threadCount);
	}

	_declspec(dllexport) void AddTileToDomain(unsigned long tilesToAdd)
	{
		WFCRuleManager::AddToInitialDomain(tilesToAdd);
	}

	_declspec(dllexport) int IWFCManager_Collapse(IWFCManager* manager, unsigned long toCollapseTo, WFCPosition* position) {
		try {
			manager->CollapseSpecificCell(position, toCollapseTo);
		}
		catch (int number) {
			return number;
		}
		return 0;
	}

	_declspec(dllexport) int IWFCManager_Run(IWFCManager* manager) {
		try {
			manager->Generate();
		}
		catch (int number) {
			return number;
		}
		return 0;
	}

	_declspec(dllexport) void WFCRule_Add_CellIsNot(unsigned long tile, unsigned long goal, unsigned int localTargetCount, WFCPosition localTargets[])
	{
		std::vector<WFCPosition*>localTargetsVec {};
		for (unsigned int i = 0; i < localTargetCount; ++i) {
			localTargetsVec.push_back(new WFCPosition { localTargets[i] });
		}
		WFCRuleManager::AddRuleToTile(tile, new WFCRuleCellIsNot(goal, tile, localTargetsVec));
	}

}