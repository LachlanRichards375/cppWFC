#pragma once
#include <memory>
#include "IWFCCollapseMethod.h"
#include "IWFCManager.h"
#include "WFCCell.h"
#include "../tracy/public/tracy/Tracy.hpp"
#include <iostream>

IWFCCollapseMethod::IWFCCollapseMethod(){
	manager = nullptr;
	continueThreadWork = true;
	JobsInQueue = 0;
}

IWFCCollapseMethod::~IWFCCollapseMethod()
{
	continueThreadWork = false;
}

void IWFCCollapseMethod::Initialize(IWFCManager* manager)
{
	IWFCCollapseMethod::manager = manager;
}

void IWFCCollapseMethod::Enqueue(WFCCell* position, std::optional<unsigned long> toCollapseTo)
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
	std::vector<unsigned long> dirtyDomain{};
	for (auto& cell : toAlert)
	{
		ZoneScopedN("Alerting Cell");
		unsigned long domain = cell->CalculateEntropy();
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

	--JobsInQueue;

}
//THIS FUCKER RIGHT HERE
//This will get optemized out so we need to move it into a seperate function
//and tell c++ not to optemize it
void IWFCCollapseMethod::WaitForJobsToFinish()
{
	while (JobsInQueue > 0) {
		if (errorMessages.getCount() > 0) {
			throw errorMessages.dequeue();
		}
	}
}
void IWFCCollapseMethod::AddJobToQueue(const std::function<void()>& job)
{
	++JobsInQueue;
	manager->QueueJobToThreadPool(job);
}
void IWFCCollapseMethod::Collapse(WFCCell* position)
{
	Enqueue(position, std::optional<unsigned long>());

	WaitForJobsToFinish();
}

void IWFCCollapseMethod::CollapseSpecificCell(WFCCell* position, unsigned long collapseTo)
{
	Enqueue(position, std::optional<unsigned long>(collapseTo));

	WaitForJobsToFinish();
}


void IWFCCollapseMethod::Reset()
{
}


