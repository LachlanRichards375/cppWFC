#pragma once
#include "IWFCManager.h"
#include "Grid2D.h"
#include "IWFCCollapseMethod.h"

extern "C" IWFCCollapseMethod * Threaded2DCollapse_Create();

extern "C" Grid2D * Grid2D_Create(unsigned int x, unsigned int y);

extern "C" IWFCManager * IWFCManager_Create(IWFCCollapseMethod * collapseMethod, IWFCGrid * grid);

extern "C" void IWFCManager_Run(IWFCManager * manager);