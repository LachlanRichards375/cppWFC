#pragma once
#include <memory>
#include "IWFCCollapseMethod.h"
#include "IWFCManager.h"
#include "WFCCell.h"

IWFCCollapseMethod::IWFCCollapseMethod(){
	manager = nullptr;
}

IWFCCollapseMethod::~IWFCCollapseMethod()
{
}

void IWFCCollapseMethod::SetManager(IWFCManager* manager)
{
	IWFCCollapseMethod::manager = manager;
}

void IWFCCollapseMethod::Enqueue(std::shared_ptr<WFCPosition> position, std::optional<unsigned long> toCollapseTo)
{
	if (toCollapseTo.has_value()) {
		updateQueue.enqueue(manager->GetCell(position)->Collapse(toCollapseTo.value()));
	}
	else {
		updateQueue.enqueue(manager->GetCell(position)->Collapse());
	}
}

void IWFCCollapseMethod::CollapseThreadWork()
{
	while (updateQueue.getCount() > 0) {
		WFCCellUpdate& cellUpdate{ updateQueue.dequeue() };

		const WFCPosition& cellUpdatePosition{ cellUpdate.updatedCell };

		std::vector<std::shared_ptr<WFCCell>> toAlert = manager->GetAlertees(cellUpdatePosition); //cellsToUpdate[cellUpdatePosition.x][cellUpdatePosition.y];
		for (auto& cell : toAlert)
		{
			std::optional<WFCCellUpdate> updateMessage = cell->DomainCheck(cellUpdate);
			if (updateMessage.has_value()) {
				updateQueue.enqueue(updateMessage.value());
			}
		}
	}
}

std::vector<WFCPosition> IWFCCollapseMethod::Collapse(std::shared_ptr<WFCPosition> position)
{
	Enqueue(position, std::optional<unsigned long>());

	while (updateQueue.getCount() > 0) {
		//Single thread to get it working
		CollapseThreadWork();
	}

	std::vector temp = std::vector<WFCPosition>(dirtyPositions);
	dirtyPositions = std::vector<WFCPosition>();
	return temp;
}

std::vector<WFCPosition> IWFCCollapseMethod::CollapseSpecificCell(std::shared_ptr<WFCPosition> position, unsigned long collapseTo)
{
	Enqueue(position, std::optional<unsigned long>(collapseTo));

	while (updateQueue.getCount() > 0) {
		//Single thread to get it working
		CollapseThreadWork();
	}

	std::vector temp = std::vector<WFCPosition>(dirtyPositions);
	dirtyPositions = std::vector<WFCPosition>();
	return temp;
}


void IWFCCollapseMethod::Reset()
{
}


