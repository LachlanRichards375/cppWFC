#pragma once
#include <memory>
#include "IWFCCollapseMethod.h"
#include "IWFCManager.h"
#include "WFCCell.h"

IWFCCollapseMethod::IWFCCollapseMethod(){
	manager = nullptr;
	continueThreadWork = true;
}

IWFCCollapseMethod::~IWFCCollapseMethod()
{
	continueThreadWork = false;
}

void IWFCCollapseMethod::Initialize(IWFCManager* manager, short threadCount)
{
	IWFCCollapseMethod::manager = manager;
	StartThreads(threadCount);
}

void IWFCCollapseMethod::Enqueue(WFCCell* position, std::optional<unsigned long> toCollapseTo)
{
	if (toCollapseTo.has_value()) {
		updateQueue.enqueue(position->Collapse(toCollapseTo.value()));
	}
	else {
		updateQueue.enqueue(position->Collapse());
	}
}

void IWFCCollapseMethod::CollapseThreadWork()
{
	while (continueThreadWork) {
		//while (updateQueue.getCount() > 0) {
			WFCCellUpdate* cellUpdate{ updateQueue.dequeue() };

			const WFCPosition* cellUpdatePosition{ (*cellUpdate).updatedCell };

			std::vector<WFCCell*> toAlert = manager->GetAlertees(cellUpdatePosition);
			for (auto& cell : toAlert)
			{
				WFCCellUpdate* updateMessage = cell->DomainCheck(cellUpdate);
				if (updateMessage != nullptr) {
					updateQueue.enqueue(updateMessage);
				}
			}
		//}
	}
}

void IWFCCollapseMethod::StartThreads(short numThredsToStart)
{
	if (numThredsToStart < 1) {
		numThredsToStart = 1;
	}
	threads.resize(numThredsToStart);
	for (int i = 0; i < numThredsToStart; ++i) {
		threads[i] = std::thread::thread(&IWFCCollapseMethod::CollapseThreadWork, this);
	}
}

std::vector<WFCPosition> IWFCCollapseMethod::Collapse(WFCCell* position)
{
	Enqueue(position, std::optional<unsigned long>());

	while (updateQueue.getCount() > 0) {
		//Single thread to get it working
		//CollapseThreadWork();
		//This should be threaded now
	}

	std::vector temp = std::vector<WFCPosition>(dirtyPositions);
	dirtyPositions = std::vector<WFCPosition>();
	return temp;
}

std::vector<WFCPosition> IWFCCollapseMethod::CollapseSpecificCell(WFCCell* position, unsigned long collapseTo)
{
	Enqueue(position, std::optional<unsigned long>(collapseTo));

	while (updateQueue.getCount() > 0) {
		//Single thread to get it working
		//CollapseThreadWork();
	}

	std::vector temp = std::vector<WFCPosition>(dirtyPositions);
	dirtyPositions = std::vector<WFCPosition>();
	return temp;
}


void IWFCCollapseMethod::Reset()
{
}


