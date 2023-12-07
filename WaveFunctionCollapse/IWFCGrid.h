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
	WFCCell* PopSpecificCell(WFCPosition* position, int bitsInDomain);
	size_t RemainingCellsToCollapse();

	//Abstract methods
	virtual WFCPosition& GetSize() const; 
	virtual WFCCell* GetCell(WFCPosition* position) = 0;
	virtual std::vector<WFCCell*> GetAlertees(const WFCPosition* positionOfInterest) = 0;
	virtual void RegisterForCellUpdates(WFCPosition* positionOfInterest, WFCCell* toRegister) = 0;
	virtual void DeRegisterForCellUpdates(WFCPosition* positionOfInterest, WFCCell* toDeregister) = 0;
	virtual void PrintGrid() = 0;
	void MarkDirty(unsigned long oldDomainCount, int index);
};