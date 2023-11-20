#pragma once
#include <memory>
#include <vector>
#include "WFCPosition.h"
#include <queue>
#include <set>
#include "SortedVector.h"
class WFCCell;
class IWFCManager;
class IWFCGrid
{
protected:
	WFCPosition& size;
	IWFCManager* manager;

	SortedVector entropyQueue{};

public:
	//Constructors
	IWFCGrid(WFCPosition& newSize);
	virtual void Initialize(IWFCManager* manager) = 0;

	//Concrete Methods
	void SortQueue();
	WFCCell* PopNextCellToCollapse();
	size_t RemainingCellsToCollapse();

	//Abstract methods
	virtual WFCPosition& GetSize() const; 
	virtual WFCCell* GetCell(std::shared_ptr <WFCPosition> position) = 0;
	virtual std::vector<WFCCell*> GetAlertees(const WFCPosition& positionOfInterest) = 0;
	virtual void RegisterForCellUpdates(std::shared_ptr<WFCPosition> positionOfInterest, WFCCell* toRegister) = 0;
	virtual void DeRegisterForCellUpdates(std::shared_ptr<WFCPosition> positionOfInterest, WFCCell* toDeregister) = 0;
	virtual void PrintGrid() = 0;
};