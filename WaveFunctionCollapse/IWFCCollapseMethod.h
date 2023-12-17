#pragma once
class IWFCManager;
class WFCCell;
struct WFCPosition;
struct WFCCellUpdate;
#include "ThreadSafeQueue.h"
#include <tuple>
#include <optional>
#include <set>
#include <functional>

class IWFCCollapseMethod
{
protected:
	IWFCManager* manager;
	bool continueThreadWork;
	int JobsInQueue;
	SafeQueue<int> errorMessages;

	void Enqueue(WFCCell* position, std::optional<unsigned long> toCollapseTo);
	void ThreadWork(WFCCellUpdate* cellUpdate);
	
	std::mutex job_count_mutex;
	void IncrementJobsInQueue();
	void DecrementJobsInQueue();

	void WaitForJobsToFinish();
	void AddJobToQueue(const std::function<void()>& job);

public:
	IWFCCollapseMethod();
	~IWFCCollapseMethod();
	void Initialize(IWFCManager* manager);
	void Collapse(WFCCell* position);
	void CollapseSpecificCell(WFCCell* position, unsigned long collapseTo);
	void Reset();
};
