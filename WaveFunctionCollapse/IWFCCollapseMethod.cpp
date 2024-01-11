#pragma once
#include <memory>
#include "IWFCCollapseMethod.h"
#include "IWFCManager.h"
#include "WFCCell.h"
#include "../tracy/public/tracy/Tracy.hpp"
#include <iostream>

IWFCCollapseMethod::IWFCCollapseMethod(){
	manager = nullptr;
	JobsInQueue = 0;
}

IWFCCollapseMethod::~IWFCCollapseMethod()
{
}

void IWFCCollapseMethod::Initialize(IWFCManager* manager)
{
	IWFCCollapseMethod::manager = manager;
}

void IWFCCollapseMethod::Enqueue(WFCCell* position, std::optional<unsigned long long > toCollapseTo)
{
	if (toCollapseTo.has_value()) {
		AddJobToQueue([this, position, toCollapseTo] { ThreadWork(position->Collapse(toCollapseTo.value())); });
	}
	else {
		AddJobToQueue([this, position] { ThreadWork(position->Collapse()); });
	}
}

void IWFCCollapseMethod::ThreadWork(WFCCellUpdate* cellUpdate) {
	ZoneScopedN("Thread CellUpdate");

	const WFCPosition* cellUpdatePosition{ (*cellUpdate).updatedCell };

	std::vector<WFCCell*> toAlert = manager->GetAlertees(cellUpdatePosition);
	std::vector<int> dirtyIndex{};
	std::vector<unsigned long long> dirtyDomain{};
	for (auto& cell : toAlert)
	{
		ZoneScopedN("Alerting Cell");
		unsigned long long domain = cell->CalculateEntropy();
		WFCCellUpdate* updateMessage = cell->DomainCheck(cellUpdate);
		if (cell->GetError() != 0) {
			errorMessages.enqueue(cell->GetError());
		}
		if (updateMessage != nullptr) {
			ZoneScopedN("Requeing Update");
			dirtyDomain.push_back(domain);
			dirtyIndex.push_back(cell->GetEntropyID());
			AddJobToQueue([this, updateMessage] { ThreadWork(updateMessage); });
		}
	}

	for (int i = 0; i < dirtyIndex.size(); ++i) {
		manager->MarkDirty(dirtyDomain[i], dirtyIndex[i]);
	}

	DecrementJobsInQueue();
}
//THIS RIGHT HERE
//This will get optimized out so we need to move it into a seperate function
//and tell c++ not to optemize it
#pragma optimize( "", off )
void IWFCCollapseMethod::IncrementJobsInQueue() {
	{
		std::unique_lock<std::mutex> lock(job_count_mutex);
		++JobsInQueue;
	}
}
void IWFCCollapseMethod::DecrementJobsInQueue() {
	{
		std::unique_lock<std::mutex> lock(job_count_mutex);
		--JobsInQueue;
	}
}
void IWFCCollapseMethod::WaitForJobsToFinish()
{
	while (JobsInQueue > 0) {
		if (errorMessages.getCount() > 0) {
			throw errorMessages.dequeue();
		}
	}
}
#pragma optimize( "", on)
void IWFCCollapseMethod::AddJobToQueue(const std::function<void()>& job)
{
	IncrementJobsInQueue();
	manager->QueueJobToThreadPool(job);
}
void IWFCCollapseMethod::Collapse(WFCCell* position)
{
	{

		ZoneScopedN("Enquing");
		Enqueue(position, std::optional<unsigned long long>());
	}
	{
		ZoneScopedN("Waiting For Jobs To Finish");
		WaitForJobsToFinish();
	}
}

void IWFCCollapseMethod::CollapseSpecificCell(WFCCell* position, unsigned long long collapseTo)
{
	Enqueue(position, std::optional<unsigned long long>(collapseTo));

	WaitForJobsToFinish();
}


void IWFCCollapseMethod::Reset()
{
	JobsInQueue = 0;
}


