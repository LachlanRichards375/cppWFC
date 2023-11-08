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

	void Enqueue(std::shared_ptr<WFCPosition> position, std::optional<unsigned long> toCollapseTo);
	SafeQueue<WFCCellUpdate> updateQueue;
	std::vector<WFCPosition> dirtyPositions;
	std::set<std::tuple<WFCPosition, std::vector<WFCCell>>> cellUpdates;

public:
	IWFCCollapseMethod(IWFCManager& manager);
	~IWFCCollapseMethod();
	std::vector<WFCPosition> Collapse(std::shared_ptr<WFCPosition> position);
	std::vector<WFCPosition> CollapseSpecificCell(std::shared_ptr<WFCPosition> position, unsigned long collapseTo);
	void RegisterForCellUpdates(WFCPosition positionOfInterest, WFCCell toRegister);
	void DeRegisterForCellUpdates(WFCPosition positionOfInterest, WFCCell toDeregister);
	void Reset();
};
