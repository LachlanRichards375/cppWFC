#include "Grid2D.h"
#include "WFCCell.h"

Grid2D::Grid2D(WFCPosition& newSize)
	:IWFCGrid(newSize)
{
}

void Grid2D::RuleSetup()
{
	grid.resize(size.x, std::vector<std::shared_ptr<WFCCell>>(size.y));

	for (int x = 0; x < size.x; ++x) {
		for (int y = 0; y < size.y; ++y){
			grid[x][y] = std::make_shared<WFCCell>(*manager, WFCPosition(x, y), (unsigned long)0);
			grid[x][y]->RuleSetup();
		}
	}
}

inline int Grid2D::RemainingCellsToCollapse()
{
	return entropyQueue.size();
}

std::shared_ptr<WFCPosition> Grid2D::PopNextCellToCollapse() {
	return nullptr;
}

inline std::shared_ptr<WFCCell> Grid2D::GetCell(std::shared_ptr <WFCPosition> position) {
	return grid[position->x][position->y];
}

inline std::vector<std::shared_ptr<WFCCell>> Grid2D::GetAlertees(const WFCPosition& positionOfInterest) {
	return cellsToUpdate[positionOfInterest.x][positionOfInterest.y];
}

void Grid2D::RegisterForCellUpdates(std::shared_ptr<WFCPosition> positionOfInterest, std::shared_ptr<WFCCell> toRegister)
{
	std::vector insertInto{cellsToUpdate[positionOfInterest->x][positionOfInterest->y]};
	std::vector<std::shared_ptr<WFCCell>>::iterator it = insertInto.begin();
	insertInto.insert(it, toRegister);
}

void Grid2D::DeRegisterForCellUpdates(std::shared_ptr<WFCPosition> positionOfInterest, std::shared_ptr<WFCCell> toDeregister)
{
	std::vector removeFrom{cellsToUpdate[positionOfInterest->x][positionOfInterest->y]};
	std::vector<std::shared_ptr<WFCCell>>::iterator it = removeFrom.begin();
	int index = 0;
	while (index < removeFrom.size()) {
		if (removeFrom[index] == toDeregister) {
			break;
		}
	}
	removeFrom.erase(it + index);
}
