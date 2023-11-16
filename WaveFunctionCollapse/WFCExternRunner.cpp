#include "WFCExternRunner.h"

extern "C" IWFCCollapseMethod* Threaded2DCollapse_Create() {
	return new IWFCCollapseMethod();
}

extern "C" Grid2D* Grid2D_Create(unsigned int x, unsigned int y) {
	return new Grid2D(*new WFCPosition(x, y));
}

extern "C" IWFCManager* IWFCManager_Create(IWFCCollapseMethod* collapseMethod, IWFCGrid* grid) {
	return new IWFCManager(collapseMethod, grid);
}

extern "C" void IWFCManager_Run(IWFCManager * manager) {
	manager->Generate();
}