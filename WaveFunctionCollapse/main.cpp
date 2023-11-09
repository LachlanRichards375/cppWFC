#pragma once
#include "WFCExternRunner.h"

int main(int argc, char* argv[]) {
	IWFCCollapseMethod* collapse = Threaded2DCollapse_Create();
	IWFCGrid* grid = Grid2D_Create(5, 5);
	IWFCManager* manager = IWFCManager_Create(collapse, grid);

	IWFCManager_Run(manager);

	return 0;
}