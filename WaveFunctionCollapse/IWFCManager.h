#pragma once
#include <memory>
#include "WFCposition.h"
#include <vector>
#include "ThreadPool.h"
struct WFCCellUpdate;
class IWFCGrid;
class IWFCCollapseMethod;
class WFCCell;

class IWFCManager
{
protected:
	void Collapse();

	IWFCGrid* _grid;
	IWFCCollapseMethod* _collapseMethod;
	ThreadPool _threadPool;
	short threadCount;

public:
	IWFCManager(IWFCCollapseMethod* collapse, IWFCGrid* grid, short threadCount);
	~IWFCManager();

	void PrintGrid();

	void Reset();
	void GenerateOnce();
	void CollapseSpecificCell(WFCPosition* position, unsigned long long toCollapseTo);
	WFCCell* GetCell(WFCPosition* position);
	WFCPosition& GetGridSize();
	std::vector<const WFCPosition*> GetLowestEntropy();
	void QueueJobToThreadPool(const std::function<void()>& job);
	bool IsThreadPoolBusy();
	void RegisterForAlert(WFCPosition* position, WFCCell* alertee);
	std::vector<WFCCell*> GetAlertees(const WFCPosition* position);
	void Generate();
	void MarkDirty(unsigned long long oldDomainCount, int index);
	int RemainingCellsToCollapse();
	std::vector<unsigned long long>* Export();
};