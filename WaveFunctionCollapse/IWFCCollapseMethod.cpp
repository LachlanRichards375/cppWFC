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
}

IWFCCollapseMethod::~IWFCCollapseMethod()
{
	continueThreadWork = false;
}

void IWFCCollapseMethod::Initialize(IWFCManager* manager)
{
	IWFCCollapseMethod::manager = manager;
}

int JobsInQueue = 0;
void IWFCCollapseMethod::Enqueue(WFCCell* position, std::optional<unsigned long> toCollapseTo)
{
	++JobsInQueue;
	if (toCollapseTo.has_value()) {
		manager->QueueJobToThreadPool([this, position, toCollapseTo] { ThreadWork(position->Collapse(toCollapseTo.value())); });
	}
	else {
		manager->QueueJobToThreadPool([this, position] { ThreadWork(position->Collapse()); });
	}
}

void IWFCCollapseMethod::ThreadWork(WFCCellUpdate* cellUpdate) {
	ZoneScopedN("Thread CellUpdate");

	const WFCPosition* cellUpdatePosition{ (*cellUpdate).updatedCell };

	std::vector<WFCCell*> toAlert = manager->GetAlertees(cellUpdatePosition);
	std::vector<int> dirtyIndex{};
	std::vector<unsigned long> dirtyDomain{};
	std::vector<WFCCell*> dirty{};
	for (auto& cell : toAlert)
	{
		ZoneScopedN("Alerting Cell");
		unsigned long domain = cell->CalculateEntropy();
		WFCCellUpdate* updateMessage = cell->DomainCheck(cellUpdate);
		if (updateMessage != nullptr) {
			ZoneScopedN("Requeing Update");
			++JobsInQueue;
			manager->QueueJobToThreadPool([this, updateMessage]{ ThreadWork(updateMessage); });
			dirtyDomain.push_back(domain);
			dirtyIndex.push_back(cell->GetEntropyID());
			dirty.push_back(cell);
		}
	}

	for (int i = 0; i < dirtyIndex.size(); ++i) {
		std::cout << "cell at " << dirty[i]->GetPosition()->to_string() << "is dirty, old values: " << dirtyDomain[i] << "," << dirtyIndex[i] << std::endl;
		manager->MarkDirty(dirtyDomain[i], dirtyIndex[i]);
	}

	dirtyIndex.clear();
	dirtyDomain.clear();
	dirty.clear();

	--JobsInQueue;
}

std::vector<WFCPosition> IWFCCollapseMethod::Collapse(WFCCell* position)
{
	Enqueue(position, std::optional<unsigned long>());

	while (JobsInQueue > 0) {
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

	while (JobsInQueue > 0) {
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


