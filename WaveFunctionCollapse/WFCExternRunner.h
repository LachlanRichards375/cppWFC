#pragma once
#include "IWFCManager.h"
#include "Grid2D.h"
#include "IWFCCollapseMethod.h"
#include "IWFCRule.h"
#include "WFCPosition.h"


extern "C" {
	IWFCCollapseMethod* Threaded2DCollapse_Create();

	Grid2D * Grid2D_Create(WFCPosition * position);

	WFCPosition * WFCPosition_Create2D(unsigned int x, unsigned int y);
	WFCPosition * WFCPosition_Create3D(unsigned int x, unsigned int y, unsigned int z);
	WFCPosition * WFCPosition_Create4D(unsigned int x, unsigned int y, unsigned int z, unsigned int w);

	IWFCManager * IWFCManager_Create(IWFCCollapseMethod * collapseMethod, IWFCGrid * grid);

	void IWFCManager_Run(IWFCManager * manager);

	void WFCRule_Add_CellIsNot(unsigned long tile, unsigned long goal, WFCPosition * localTargets[]);
}