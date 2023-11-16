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
	IWFCManager* manager;

	SafeQueue<WFCCellUpdate> updateQueue;
	std::vector<WFCPosition> dirtyPositions;

	void Enqueue(std::shared_ptr<WFCCell> position, std::optional<unsigned long> toCollapseTo);
	void CollapseThreadWork();

public:
	IWFCCollapseMethod();
	~IWFCCollapseMethod();
	void SetManager(IWFCManager* manager);
	std::vector<WFCPosition> Collapse(std::shared_ptr<WFCCell> position);
	std::vector<WFCPosition> CollapseSpecificCell(std::shared_ptr<WFCCell> position, unsigned long collapseTo);
	void Reset();
};
