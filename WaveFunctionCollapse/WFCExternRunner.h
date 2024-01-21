#pragma once
#include "IWFCManager.h"
#include "Grid2D.h"
#include "IWFCCollapseMethod.h"
#include "IWFCRule.h"
#include "WFCPosition.h"


extern "C" {
	_declspec(dllexport) IWFCCollapseMethod* Threaded2DCollapse_Create();

	_declspec(dllexport) Grid2D * Grid2D_Create(WFCPosition * position);

	_declspec(dllexport) WFCPosition * WFCPosition_Create2D(unsigned int x, unsigned int y);
	_declspec(dllexport) WFCPosition * WFCPosition_Create3D(unsigned int x, unsigned int y, unsigned int z);
	_declspec(dllexport) WFCPosition * WFCPosition_Create4D(unsigned int x, unsigned int y, unsigned int z, unsigned int w);

	_declspec(dllexport) IWFCManager * IWFCManager_Create(IWFCCollapseMethod * collapseMethod, IWFCGrid * grid, short threadCount);

	_declspec(dllexport) void AddTileToDomain(unsigned long long tilesToAdd);

	_declspec(dllexport) int IWFCManager_Collapse(IWFCManager* manager, unsigned long long toCollapseTo, WFCPosition* position);
	_declspec(dllexport) int IWFCManager_Run(IWFCManager* manager);
	_declspec(dllexport) bool IWFCManager_GetResult(IWFCManager* manager, unsigned long long * arrayToFill, int length);

	_declspec(dllexport) void WFCRule_Add_CellIsNot(unsigned long long tile, unsigned long long goal, WFCPosition* localTargets, unsigned int localTargetCount);
}