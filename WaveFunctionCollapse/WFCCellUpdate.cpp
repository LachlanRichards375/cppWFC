#include "WFCCellUpdate.h"
#include "WFCCell.h"

WFCCellUpdate::WFCCellUpdate(unsigned long long removed, unsigned long long added, unsigned long long collapsed, const WFCPosition* position):
	updatedCell(position)
{
	removedFromDomain = removed;
	addedToDomain = added;
	collapsedTo = collapsed;
}
