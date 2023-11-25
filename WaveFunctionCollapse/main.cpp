#pragma once
#include "WFCExternRunner.h"
#include <iostream>

const unsigned long GRASS = 1<<0;
const unsigned long SAND  = 1<<1;
const unsigned long WATER = 1<<2;


void createRules() {
	//1 Grass
	//2 Sand
	//4 Water
	WFCPosition IgnoreWest[]{ new WFCPosition(-1,0), new WFCPosition(-1,1), new WFCPosition(-1,-1) };
	WFCPosition IgnoreEast[]{ new WFCPosition(1,0), new WFCPosition(1,1), new WFCPosition(1,-1) };
	WFCPosition IgnoreAllAround[]{ new WFCPosition(1,0), new WFCPosition(1,1), new WFCPosition(1,-1), new WFCPosition(-1,0), new WFCPosition(-1,1), new WFCPosition(-1,-1), new WFCPosition(0,1), new WFCPosition(0,-1) };

	//Grass cant have Sand West
	WFCRule_Add_CellIsNot(GRASS, SAND, 3, IgnoreWest);
	//Grass can't have Water anywhere
	WFCRule_Add_CellIsNot(GRASS, WATER, 8, IgnoreAllAround);

	//Sand can't have Water West
	WFCRule_Add_CellIsNot(SAND, WATER, 3, IgnoreWest);
	//Sand can't have Grass East
	WFCRule_Add_CellIsNot(SAND, GRASS, 3, IgnoreEast);

	//Water cant have Sand East
	WFCRule_Add_CellIsNot(WATER, SAND, 3, IgnoreEast);
	//Water can't have Grass anywhere
	WFCRule_Add_CellIsNot(WATER, GRASS, 8, IgnoreAllAround);
}

int main(int argc, char* argv[]) {

	
	auto t1 = std::chrono::high_resolution_clock::now();
	
	AddTileToDomain((1 + 2 + 4));
	createRules();

	IWFCCollapseMethod* collapse = Threaded2DCollapse_Create();
	IWFCGrid* grid = Grid2D_Create(new WFCPosition(10, 10));
	IWFCManager* manager = IWFCManager_Create(collapse, grid);

	IWFCManager_Collapse(manager, SAND, new WFCPosition(5,5));

	IWFCManager_Run(manager);

	auto t2 = std::chrono::high_resolution_clock::now();

	/* Getting number of milliseconds as a double. */
	std::chrono::duration<double, std::milli> ms_double = t2 - t1;

	std::cout << ms_double.count() << "ms" << std::endl;

	manager->PrintGrid();

	system("pause");

	return 0;
}