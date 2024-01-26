#include "Grid2D.h"
#include "WFCCell.h"
#include <iostream>
#include "WFCRuleManager.h"
#include "SortedVector.h"
#include <string>
#include "../tracy/public/tracy/Tracy.hpp"

Grid2D::Grid2D(WFCPosition& newSize)
	:IWFCGrid(newSize)
{
	
}

void Grid2D::Initialize(IWFCManager* newManager) {
	ZoneScopedN("Grid2D Initialize");
	//Set manager
	IWFCGrid::manager = newManager;

	{
		ZoneScopedN("Resizing grid and cells to update");
		//Set grid size
		grid.resize(size.x, std::vector<WFCCell*>(size.y));
		//Initialize cells to update list
		cellsToUpdate.resize(size.x, std::vector<std::unordered_set<WFCCell*>>(size.y));
	}

	{
		ZoneScopedN("Print size of grid");
		std::string output = "Creating grid of size: ";
		output.append(std::to_string(size.x).append(",").append(std::to_string(size.y)));
		std::cout << output << std::endl;
	}

	{
		ZoneScopedN("Filling the cells");
		for (int x = 0; x < size.x; ++x) {
			for (int y = 0; y < size.y; ++y) {
				WFCCell* cell;
				{
					ZoneScopedN("Create New Cell");
					cell = new WFCCell(manager, new WFCPosition{ x, y }, WFCRuleManager::GetInitialDomain());
				}
				grid[x][y] = cell;
				{
					ZoneScopedN("Rule Setup");
					cell->RuleSetup();
				}
				entropyQueue.insert(cell);
			}
		}
	}
}

inline WFCCell* Grid2D::GetCell(WFCPosition* position) {
	return grid[position->x][position->y];
}

inline std::unordered_set<WFCCell*> Grid2D::GetAlertees(const WFCPosition* positionOfInterest) {
	return cellsToUpdate[positionOfInterest->x][positionOfInterest->y];
}

void Grid2D::RegisterForCellUpdates(WFCPosition* positionOfInterest, WFCCell* toRegister)
{
	ZoneScopedN("RegisterForCellUpdates");
	if (positionOfInterest->x >= 0 && positionOfInterest->x < size.x) {
		if (positionOfInterest->y >= 0 && positionOfInterest->y < size.y) {
			cellsToUpdate[positionOfInterest->x][positionOfInterest->y].insert(toRegister);
		}
	}
}

void Grid2D::DeRegisterForCellUpdates(WFCPosition* positionOfInterest, WFCCell* toDeregister)
{
	if (positionOfInterest->x > 0 && positionOfInterest->x <= size.x) {
		if (positionOfInterest->y > 0 && positionOfInterest->y <= size.y) {
			cellsToUpdate[positionOfInterest->x][positionOfInterest->y].erase(toDeregister);
		}
	}
}

void Grid2D::PrintGrid()
{
	std::cout << "Printing 2D grid: " << std::endl;
	for(int row = size.x-1; row>=0;--row){ //0 bottom left
	//for (int x = 0; x < size.x; ++x) { //0 top left
		std::string output = "|";
		for (int col = 0; col < size.y; ++col) {
			unsigned long long collapsedTile = grid[col][row]->CollapsedTile;

			if (collapsedTile < 100) {
				output.append(" ");
			}

			output.append(std::to_string(collapsedTile));

			if (collapsedTile < 10) {
				output.append(" ");
			}
		}
		output.append("|");
		std::cout << output << std::endl;
	}
}

void Grid2D::Reset() {
	//Set grid size
	grid.clear();
	grid.resize(size.x, std::vector<WFCCell*>(size.y));
	//clear cells to update list
	cellsToUpdate.clear();
	cellsToUpdate.resize(size.x, std::vector<std::unordered_set<WFCCell*>>(size.y));
	
	entropyQueue.clear();

	std::string output = "Creating grid of size: ";
	output.append(std::to_string(size.x).append(",").append(std::to_string(size.y)));
	std::cout << output << std::endl;
	for (int x = 0; x < size.x; ++x) {
		for (int y = 0; y < size.y; ++y) {
			WFCCell* cell = new WFCCell(manager, new WFCPosition{ x, y }, WFCRuleManager::GetInitialDomain());
			grid[x][y] = cell;
			cell->RuleSetup();
			entropyQueue.insert(cell);
		}
	}
}


std::vector<unsigned long long > exporter;
std::vector<unsigned long long>* Grid2D::Export() {
	exporter.clear();
	for (auto& row : grid) {
		for (auto& column : row) {
			exporter.push_back(column->CollapsedTile);
		}
	}

	return &exporter;
}
