#include "WFCCellUpdate.h"
#include "WFCCell.h"

WFCCellUpdate::WFCCellUpdate(unsigned long removed, unsigned long added, unsigned long collapsed, const WFCPosition& position):
	updatedCell(position)
{
	removedFromDomain = removed;
	addedToDomain = added;
	collapsedTo = collapsed;
}
