#pragma once
#include <memory>
#include "WFCPosition.h"
class WFCCell;

class IWFCGrid
{
protected:
	WFCPosition size;
public:
	virtual std::shared_ptr<WFCPosition> PopNextCellToCollapse() = 0;
	virtual std::shared_ptr<WFCCell> GetCell(std::shared_ptr <WFCPosition> position) = 0;
	virtual WFCPosition& GetSize();
};