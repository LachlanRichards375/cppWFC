#pragma once
#include <memory>
#include <vector>
#include "WFCPosition.h"
class WFCCell;

class IWFCGrid
{
protected:
	WFCPosition& size;
public:
	IWFCGrid(WFCPosition& newSize);
	virtual WFCPosition& GetSize();
	virtual int RemainingCellsToCollapse() = 0;
	virtual std::shared_ptr<WFCPosition> PopNextCellToCollapse() = 0;
	virtual std::shared_ptr<WFCCell> GetCell(std::shared_ptr <WFCPosition> position) = 0;
	virtual std::vector<std::shared_ptr<WFCCell>> GetAlertees(WFCPosition& positionOfInterest) = 0;
	virtual void RegisterForCellUpdates(std::shared_ptr<WFCPosition> positionOfInterest, std::shared_ptr <WFCCell> toRegister) = 0;
	virtual void DeRegisterForCellUpdates(std::shared_ptr<WFCPosition> positionOfInterest, std::shared_ptr <WFCCell> toDeregister) = 0;
};