#pragma once
#include <memory>
#include "IWFCCollapseMethod.h"
#include "IWFCManager.h"
#include "WFCCell.h"
#include "../tracy/public/tracy/Tracy.hpp"

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

	ZoneScopedN("CollapseMethod Initialize");
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
	ZoneScopedN("Thread Life");
	while (continueThreadWork) {
		//while (updateQueue.getCount() > 0) {
			WFCCellUpdate* cellUpdate{ updateQueue.dequeue() };
			{
				ZoneScopedN("Completing Thread Work");

				const WFCPosition* cellUpdatePosition{ (*cellUpdate).updatedCell };

				std::vector<WFCCell*> toAlert = manager->GetAlertees(cellUpdatePosition);
				for (auto& cell : toAlert)
				{
					WFCCellUpdate* updateMessage = cell->DomainCheck(cellUpdate);
					if (updateMessage != nullptr) {
						updateQueue.enqueue(updateMessage);
					}
				}
			}
		//}
	}
}

void IWFCCollapseMethod::StartThreads(short numThredsToStart)
{
	ZoneScopedN("Starting Threads");
	if (numThredsToStart < 1) {
		numThredsToStart = 1;
	}
	threads.resize(numThredsToStart);
	for (int i = 0; i < numThredsToStart; ++i) {
		ZoneScopedN("Initialize Thread");
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


