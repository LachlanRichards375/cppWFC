#include "WFCCellUpdate.h"
#include "WFCCell.h"

WFCCellUpdate::WFCCellUpdate(unsigned long removedFromDomain, unsigned long addedToDomain, WFCCell* updatedCell) :
	updatedCell(*updatedCell)
{
	WFCCellUpdate::removedFromDomain = removedFromDomain;
	WFCCellUpdate::addedToDomain = addedToDomain;
}
