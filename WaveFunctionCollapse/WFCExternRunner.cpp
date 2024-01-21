#include "WFCExternRunner.h"
#include "WFCRuleManager.h"
#include "WFCRuleCellIsNot.h"
#include <iostream>

extern "C"  {

	_declspec(dllexport) IWFCCollapseMethod* Threaded2DCollapse_Create() {
		return new IWFCCollapseMethod();
	}

	_declspec(dllexport) WFCPosition * WFCPosition_Create2D(int x, int y)				{ return new WFCPosition(x, y); }
	_declspec(dllexport) WFCPosition * WFCPosition_Create3D(int x, int y, int z)		{ return new WFCPosition(x, y, z); }
	_declspec(dllexport) WFCPosition * WFCPosition_Create4D(int x, int y, int z, int w)	{ return new WFCPosition(x, y, z, w); }
	
	_declspec(dllexport) Grid2D* Grid2D_Create(WFCPosition* position) {
		return new Grid2D(*position);
	}

	_declspec(dllexport) IWFCManager* IWFCManager_Create(IWFCCollapseMethod* collapseMethod, IWFCGrid* grid, short threadCount) {
		return new IWFCManager(collapseMethod, grid, threadCount);
	}

	_declspec(dllexport) void AddTileToDomain(unsigned long long tilesToAdd)
	{
		WFCRuleManager::AddToInitialDomain(tilesToAdd);
	}

	_declspec(dllexport) int IWFCManager_Collapse(IWFCManager* manager, unsigned long long toCollapseTo, WFCPosition* position) {
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

	_declspec(dllexport) bool IWFCManager_GetResult(IWFCManager* manager, unsigned long long* arrayToFill, int length)
	{
		auto vector = manager->Export();
		if (length != vector->size()) {
			std::cout << "Vector has differeing size";
			return false;
		}
		for (int i = 0; i < length; i++) {
			arrayToFill[i] = (*vector)[i];
			std::cout << "Inserting " << std::to_string((*vector)[i]) << "(";
			printf("%llu", (*vector)[i]);
			std::cout << ") to " << std::to_string(i) << "\n";
		}
		return true;
	}

	_declspec(dllexport) void WFCRule_Add_CellIsNot(unsigned long long tile, unsigned long long goal, WFCPosition* localTargets, unsigned int localTargetCount)
	{
		std::vector<WFCPosition*>localTargetsVec {};
		for (unsigned int i = 0; i < localTargetCount; ++i) {
			//Create new copy so we own the positions
			localTargetsVec.push_back(new WFCPosition { localTargets[i] });
		}
		WFCRuleManager::AddRuleToTile(tile, new WFCRuleCellIsNot(goal, tile, localTargetsVec));
	}

}