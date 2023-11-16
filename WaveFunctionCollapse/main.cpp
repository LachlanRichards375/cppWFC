#pragma once
#include "WFCExternRunner.h"


void createRules() {
	//1 Grass
	//2 Sand
	//4 Water
	WFCPosition* IgnoreWest[]{ new WFCPosition(-1,0), new WFCPosition(-1,1), new WFCPosition(-1,-1) };
	WFCPosition* IgnoreEast[]{ new WFCPosition(1,0), new WFCPosition(1,1), new WFCPosition(1,-1) };
	WFCPosition* IgnoreAllAround[]{ new WFCPosition(1,0), new WFCPosition(1,1), new WFCPosition(1,-1), new WFCPosition(-1,0), new WFCPosition(-1,1), new WFCPosition(-1,-1), new WFCPosition(0,1), new WFCPosition(0,-1) };

	//Grass cant have Sand West
	WFCRule_Add_CellIsNot((unsigned long)1, (unsigned long)2, IgnoreWest);
	//Grass can't have Water anywhere
	WFCRule_Add_CellIsNot((unsigned long)1, (unsigned long)4, IgnoreAllAround);

	//Sand can't have Water West
	WFCRule_Add_CellIsNot((unsigned long)2, (unsigned long)4, IgnoreWest);
	//Sand can't have Grass East
	WFCRule_Add_CellIsNot((unsigned long)2, (unsigned long)1, IgnoreEast);

	//Water cant have Sand East
	WFCRule_Add_CellIsNot((unsigned long)4, (unsigned long)2, IgnoreEast);
	//Water can't have Grass anywhere
	WFCRule_Add_CellIsNot((unsigned long)4, (unsigned long)1, IgnoreAllAround);
}

int main(int argc, char* argv[]) {
	
	AddTileToDomain((1 + 2 + 4));
	createRules();

	IWFCCollapseMethod* collapse = Threaded2DCollapse_Create();
	IWFCGrid* grid = Grid2D_Create(new WFCPosition(5, 5));
	IWFCManager* manager = IWFCManager_Create(collapse, grid);


	IWFCManager_Run(manager);

	system("pause");

	return 0;
}