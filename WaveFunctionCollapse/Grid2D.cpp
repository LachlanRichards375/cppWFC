#include "Grid2D.h"
#include "WFCCell.h"

std::shared_ptr<WFCPosition> Grid2D::PopNextCellToCollapse() {
	return nullptr;
}
std::shared_ptr<WFCCell> Grid2D::GetCell(std::shared_ptr <WFCPosition> position) {
	return grid[position->x][position->y];
}