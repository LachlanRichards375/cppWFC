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
	WFCPosition IgnoreAllAround[]{  new WFCPosition(-1,1), new WFCPosition(0,1), new WFCPosition(1,1), 
									new WFCPosition(-1,0),	/*	   TILE		*/	 new WFCPosition(1,0),
									new WFCPosition(-1,-1),new WFCPosition(0,-1),new WFCPosition(1,-1), };

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

	std::cout << "Cells: ";
	std::cout << "\n\t|- GRASS ->" << GRASS;
	std::cout << "\n\t|- SAND  ->" << SAND;
	std::cout << "\n\t|- WATER ->" << WATER;
	std::cout << std::endl;
	std::cout << "RULES: " << std::endl;
	std::cout << "GRASS can't have sand west (x-1), GRASS can't have water anywhere." << std::endl;
	std::cout << "SAND can't have water west (x-1), SAND can't have grass east (x+1)." << std::endl;
	std::cout << "WATER can't have sand east (x+1), WATER can't have grass anywhere." << std::endl;
}

int main(int argc, char* argv[]) {

	
	auto t1 = std::chrono::high_resolution_clock::now();
	
	WFCPosition* size = new WFCPosition(5, 5);

	AddTileToDomain(SAND + WATER + GRASS);
	createRules();

	IWFCCollapseMethod* collapse = Threaded2DCollapse_Create();
	IWFCGrid* grid = Grid2D_Create(size);
	IWFCManager* manager = IWFCManager_Create(collapse, grid, 12);

	int messageNo = IWFCManager_Collapse(manager, SAND, new WFCPosition(size->x / 2, size->y/2));
	if (messageNo != 0) {
		std::cout << "Error (" << messageNo <<") collapsing first cell to Sand. Program Aborting." << std::endl;
		return 0;
	}
	
	messageNo = IWFCManager_Run(manager);
	if (messageNo != 0) {
		std::cout << "Error Generating: (" << messageNo << ") program aborting." << std::endl;
	}

	auto t2 = std::chrono::high_resolution_clock::now();

	/* Getting number of milliseconds as a double. */
	std::chrono::duration<double, std::milli> ms_double = t2 - t1;

	std::cout << ms_double.count() << "ms" << std::endl;

	if (messageNo == 0) {
		manager->PrintGrid();
	}

	system("pause");

	return 0;
}