#pragma once
class IWFCManager;
class WFCCell;
struct WFCPosition;
struct WFCCellUpdate;
#include "ThreadSafeQueue.h"
#include <tuple>
#include <optional>
#include <set>

class IWFCCollapseMethod
{
protected:
	IWFCManager& manager;

	SafeQueue<WFCCellUpdate> updateQueue;
	std::vector<WFCPosition> dirtyPositions;
	//2d vector
	//Array of shared pointers
	std::vector<std::vector<std::vector<std::shared_ptr<WFCCell>>>> cellsToUpdate;

	void Enqueue(std::shared_ptr<WFCPosition> position, std::optional<unsigned long> toCollapseTo);
	void CollapseThreadWork();

public:
	IWFCCollapseMethod(IWFCManager& manager);
	~IWFCCollapseMethod();
	std::vector<WFCPosition> Collapse(std::shared_ptr<WFCPosition> position);
	std::vector<WFCPosition> CollapseSpecificCell(std::shared_ptr<WFCPosition> position, unsigned long collapseTo);
	void RegisterForCellUpdates(std::shared_ptr<WFCPosition> positionOfInterest, std::shared_ptr <WFCCell> toRegister);
	void DeRegisterForCellUpdates(std::shared_ptr<WFCPosition> positionOfInterest, std::shared_ptr <WFCCell> toDeregister);
	void Reset();
};
