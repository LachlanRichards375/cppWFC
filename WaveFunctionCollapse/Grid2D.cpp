#include "Grid2D.h"
#include "WFCCell.h"
#include <iostream>
#include "WFCRuleManager.h"
#include "SortedVector.h"

Grid2D::Grid2D(WFCPosition& newSize)
	:IWFCGrid(newSize)
{
	//Set grid size
	grid.resize(size.x, std::vector<WFCCell*>(size.y));
	//Initialize cells to update list
	cellsToUpdate.resize(size.x, std::vector<std::vector<WFCCell*>>(size.y));

	std::cout << "Creating grid of size: " << size.x << "," << size.y << std::endl;
	for (int x = 0; x < size.x; ++x) {
		for (int y = 0; y < size.y; ++y) {
			grid[x][y] = new WFCCell(*manager, new WFCPosition(x, y), WFCRuleManager::GetInitialDomain());
			grid[x][y]->RuleSetup();
			entropyQueue.insert(grid[x][y]);
		}
	}
}

inline WFCCell* Grid2D::GetCell(std::shared_ptr <WFCPosition> position) {
	return grid[position->x][position->y];
}

inline std::vector<WFCCell*> Grid2D::GetAlertees(const WFCPosition& positionOfInterest) {
	return cellsToUpdate[positionOfInterest.x][positionOfInterest.y];
}

void Grid2D::RegisterForCellUpdates(std::shared_ptr<WFCPosition> positionOfInterest, WFCCell* toRegister)
{
	std::vector<WFCCell*> insertInto{cellsToUpdate[positionOfInterest->x][positionOfInterest->y]};
	std::vector<WFCCell*>::iterator it = insertInto.begin();
	insertInto.insert(it, toRegister);
}

void Grid2D::DeRegisterForCellUpdates(std::shared_ptr<WFCPosition> positionOfInterest, WFCCell* toDeregister)
{
	std::vector<WFCCell*> removeFrom{cellsToUpdate[positionOfInterest->x][positionOfInterest->y]};
	std::vector<WFCCell*>::iterator it = removeFrom.begin();
	int index = 0;
	while (index < removeFrom.size()) {
		if (removeFrom[index] == toDeregister) {
			break;
		}
	}
	removeFrom.erase(it + index);
}

void Grid2D::PrintGrid()
{
	std::cout << "Printing 2D grid: " << std::endl;
	std::string divider = "";
	for (int x = 0; x < size.x; ++x) {
		std::cout << divider << std::endl;
		for (int y = 0; y < size.y; ++y) {
			std::cout << "|";
			unsigned long collapsedTile = grid[x][y]->CollapsedTile;

			if (collapsedTile < 100) {
				std::cout << " ";
			}

			std::cout << collapsedTile;

			if (collapsedTile < 10) {
				std::cout << " ";
			}
			std::cout << "|";
		}
		std::cout << std::endl;
	}
}
