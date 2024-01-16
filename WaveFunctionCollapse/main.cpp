#pragma once
#include "WFCExternRunner.h"
#include <iostream>
#include "../tracy/public/tracy/Tracy.hpp"

const unsigned long long DEEP_GRASS = 1 << 0;
const unsigned long long GRASS = 1 << 1;
const unsigned long long SAND  = 1 << 2;
const unsigned long long WATER = 1 << 3;
const unsigned long long DEEP_WATER = 1 << 4;


void createRules() {
	ZoneScopedN("Creating WFCRules");
	//1 Grass
	//2 Sand
	//4 Water
	WFCPosition IgnoreWest[]{ new WFCPosition(-1,0), new WFCPosition(-1,1), new WFCPosition(-1,-1) };
	WFCPosition IgnoreEast[]{ new WFCPosition(1,0), new WFCPosition(1,1), new WFCPosition(1,-1) };
	WFCPosition IgnoreAllAround[]{  new WFCPosition(-1,1), new WFCPosition(0,1), new WFCPosition(1,1), 
									new WFCPosition(-1,0),	/*	   TILE		*/	 new WFCPosition(1,0),
									new WFCPosition(-1,-1),new WFCPosition(0,-1),new WFCPosition(1,-1), };
	{
		ZoneScopedN("Adding Deep_Grass Rules");
		//Grass can't have Sand or Water anywhere
		WFCRule_Add_CellIsNot(DEEP_GRASS, SAND, 8, IgnoreAllAround);
		WFCRule_Add_CellIsNot(DEEP_GRASS, WATER, 8, IgnoreAllAround);
		WFCRule_Add_CellIsNot(DEEP_GRASS, DEEP_WATER, 8, IgnoreAllAround);
	}
	{
		ZoneScopedN("Adding Grass Rules");
		//Grass cant have Sand West
		WFCRule_Add_CellIsNot(GRASS, SAND, 3, IgnoreWest);
		//Grass can't have Water anywhere
		WFCRule_Add_CellIsNot(GRASS, WATER, 8, IgnoreAllAround);
		WFCRule_Add_CellIsNot(GRASS, DEEP_WATER, 8, IgnoreAllAround);
	}
	{
		ZoneScopedN("Adding Sand Rules");
		//Sand can't have Water West
		WFCRule_Add_CellIsNot(SAND, DEEP_WATER, 8, IgnoreAllAround);
		WFCRule_Add_CellIsNot(SAND, WATER, 3, IgnoreWest);
		//Sand can't have Grass East
		WFCRule_Add_CellIsNot(SAND, GRASS, 3, IgnoreEast);
		WFCRule_Add_CellIsNot(SAND, DEEP_GRASS, 8, IgnoreAllAround);

	}
	{
		ZoneScopedN("Adding Water Rules");
		//Water cant have Sand East
		WFCRule_Add_CellIsNot(WATER, SAND, 3, IgnoreEast);
		//Water can't have Grass anywhere
		WFCRule_Add_CellIsNot(WATER, GRASS, 8, IgnoreAllAround);
		WFCRule_Add_CellIsNot(WATER, DEEP_GRASS, 8, IgnoreAllAround);
	}
	{
		ZoneScopedN("Adding Water Rules");
		//Water cant have Grass or Sand Anywhere
		WFCRule_Add_CellIsNot(DEEP_WATER, SAND, 8, IgnoreAllAround);
		WFCRule_Add_CellIsNot(DEEP_WATER, GRASS, 8, IgnoreAllAround);
		WFCRule_Add_CellIsNot(DEEP_WATER, DEEP_GRASS, 8, IgnoreAllAround);
	}

	{
		ZoneScopedN("Printing tile id's to screen");
		std::string output = "Cells: ";
		output
			.append("\n\t| -DEEP_GRASS ->").append(std::to_string(DEEP_GRASS))
			.append("\n\t| -GRASS ->").append(std::to_string(GRASS))
			.append("\n\t| -SAND  ->").append(std::to_string(SAND))
			.append("\n\t| -WATER ->").append(std::to_string(WATER))
			.append("\n\t| -DEEP_WATER ->").append(std::to_string(DEEP_WATER));
		std::cout << output << std::endl;
	}
}

int main(int argc, char* argv[]) {
	IWFCManager* manager;
	int messageNo = 0;
	auto t1 = std::chrono::high_resolution_clock::now();

	{
		ZoneScopedN("Generate Result");
		WFCPosition* size = new WFCPosition(10, 10);
		IWFCCollapseMethod* collapse;
		IWFCGrid* grid;

		{
			ZoneScopedN("Initialize");

			AddTileToDomain(DEEP_GRASS + GRASS + SAND + WATER + DEEP_WATER);
			createRules();

			{
				ZoneScopedN("CollapseMethod Create");
				collapse = Threaded2DCollapse_Create();
			}
			{
				ZoneScopedN("Grid2D Create");
				grid = Grid2D_Create(size);
			}
			{
				ZoneScopedN("Manager Create");
				manager = IWFCManager_Create(collapse, grid, 2);
			}
		}

		{
			ZoneScopedN("ForceCollapse sand");
			messageNo = IWFCManager_Collapse(manager, SAND, new WFCPosition(size->x / 2, size->y / 2));
			if (messageNo != 0) {
				std::cout << "Error (" << messageNo << ") collapsing first cell to Sand. Program Aborting." << std::endl;
				return 0;
			}
		}

		{
			ZoneScopedN("Generate");
			messageNo = IWFCManager_Run(manager);
			if (messageNo != 0) {
				std::cout << "Error Generating: (" << messageNo << ") program aborting." << std::endl;
			}
		}

	}
	{
		ZoneScopedN("Printing result");

		auto t2 = std::chrono::high_resolution_clock::now();

		/* Getting number of milliseconds as a double. */
		std::chrono::duration<double, std::milli> ms_double = t2 - t1;

		std::cout << ms_double.count() << "ms" << std::endl;
		std::cout << "Remaining cells: " << std::to_string(manager->RemainingCellsToCollapse()) << "\n";
		manager->PrintGrid();
	}
	delete manager;
	return 0;
}