#include "Grid2D.h"
#include "WFCCell.h"

Grid2D::Grid2D(WFCPosition& newSize)
	:IWFCGrid(newSize)
{
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

inline std::vector<std::shared_ptr<WFCCell>> Grid2D::GetAlertees(WFCPosition& positionOfInterest) {
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
