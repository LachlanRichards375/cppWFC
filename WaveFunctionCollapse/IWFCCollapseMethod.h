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

	SafeQueue<WFCCellUpdate*> updateQueue;
	std::vector<WFCPosition> dirtyPositions;
	std::vector<std::thread> threads;
	bool continueThreadWork;

	void Enqueue(WFCCell* position, std::optional<unsigned long> toCollapseTo);
	void CollapseThreadWork();
	void StartThreads(short numThredsToStart);

public:
	IWFCCollapseMethod();
	~IWFCCollapseMethod();
	void Initialize(IWFCManager* manager, short threadCount);
	std::vector<WFCPosition> Collapse(WFCCell* position);
	std::vector<WFCPosition> CollapseSpecificCell(WFCCell* position, unsigned long collapseTo);
	void Reset();
};
