#pragma once
#include <memory>
#include "IWFCCollapseMethod.h"
#include "IWFCManager.h"
#include "WFCCell.h"

IWFCCollapseMethod::IWFCCollapseMethod(IWFCManager& manager) : manager(manager) {
	WFCPosition& gridSize{ manager.GetGridSize() };
	for (int x = 0; x < gridSize.x; ++x) {
		cellsToUpdate[x] = {};
		for (int y = 0; y < gridSize.y; ++y) {
			cellsToUpdate[x][y] = {};
		}
	}
}

IWFCCollapseMethod::~IWFCCollapseMethod()
{
}

void IWFCCollapseMethod::Enqueue(std::shared_ptr<WFCPosition> position, std::optional<unsigned long> toCollapseTo)
{
	if (toCollapseTo.has_value()) {
		updateQueue.enqueue(manager.GetCell(position)->Collapse(toCollapseTo.value()));
	}
	else {
		updateQueue.enqueue(manager.GetCell(position)->Collapse());
	}
}

void IWFCCollapseMethod::CollapseThreadWork()
{
	while (updateQueue.getCount() > 0) {
		WFCCellUpdate& cellUpdate{ updateQueue.dequeue() };

		WFCPosition& cellUpdatePosition{ cellUpdate.updatedCell.GetPosition() };

		std::vector<std::shared_ptr<WFCCell>> toAlert = cellsToUpdate[cellUpdatePosition.x][cellUpdatePosition.y];
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

void IWFCCollapseMethod::RegisterForCellUpdates(std::shared_ptr<WFCPosition> positionOfInterest, std::shared_ptr<WFCCell> toRegister)
{
	std::vector insertInto{cellsToUpdate[positionOfInterest->x][positionOfInterest->y]};
	std::vector<std::shared_ptr<WFCCell>>::iterator it = insertInto.begin();
	insertInto.insert(it, toRegister);
}

void IWFCCollapseMethod::DeRegisterForCellUpdates(std::shared_ptr<WFCPosition> positionOfInterest, std::shared_ptr <WFCCell> toDeregister)
{
	std::vector removeFrom{cellsToUpdate[positionOfInterest->x][positionOfInterest->y]};
	std::vector<std::shared_ptr<WFCCell>>::iterator it = removeFrom.begin();
	int index = 0;
	while (index < removeFrom.size()) {
		if (removeFrom[index] == toDeregister) {
			break;
		}
	}
	removeFrom.erase(it + index);
}

void IWFCCollapseMethod::Reset()
{
}


