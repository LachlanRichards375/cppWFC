#pragma once
#include <memory>
#include <vector>
#include "WFCPosition.h"
#include <queue>
#include <set>
class WFCCell;
class IWFCManager;

class IWFCGrid
{
protected:
	WFCPosition& size;
	IWFCManager* manager;

	//std::priority_queue<
	//	std::shared_ptr<WFCCell>, //Type
	//	std::vector<std::shared_ptr<WFCCell>>, //Container
	//	std::greater<WFCCell> //Makes it sort min priority
	//> entropyQueue;

	std::set<std::shared_ptr<WFCCell>, std::greater<std::shared_ptr<WFCCell>>> entropyQueue;

public:
	//Constructors
	IWFCGrid(WFCPosition& newSize);
	void SetManager(IWFCManager* manager);

	//Concrete Methods
	void SortQueue();
	std::shared_ptr<WFCCell> PopNextCellToCollapse();

	//Abstract methods
	virtual WFCPosition& GetSize() const; 
	virtual void RuleSetup() = 0;
	virtual int RemainingCellsToCollapse() = 0;
	virtual std::shared_ptr<WFCCell> GetCell(std::shared_ptr <WFCPosition> position) = 0;
	virtual std::vector<std::shared_ptr<WFCCell>> GetAlertees(const WFCPosition& positionOfInterest) = 0;
	virtual void RegisterForCellUpdates(std::shared_ptr<WFCPosition> positionOfInterest, std::shared_ptr <WFCCell> toRegister) = 0;
	virtual void DeRegisterForCellUpdates(std::shared_ptr<WFCPosition> positionOfInterest, std::shared_ptr <WFCCell> toDeregister) = 0;
};