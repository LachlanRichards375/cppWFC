#pragma once
#include <memory>
#include "IWFCCollapseMethod.h"
#include "IWFCManager.h"
#include "WFCCell.h"

IWFCCollapseMethod::IWFCCollapseMethod(IWFCManager& manager) : manager(manager) {
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

std::vector<WFCPosition> IWFCCollapseMethod::Collapse(std::shared_ptr<WFCPosition> position)
{
	Enqueue(position, std::optional<unsigned long>());

	while (updateQueue.getCount() > 0) {
			//Loop without caring
	}

	std::vector temp = std::vector<WFCPosition>(dirtyPositions);
	dirtyPositions = std::vector<WFCPosition>();
	return temp;
}

std::vector<WFCPosition> IWFCCollapseMethod::CollapseSpecificCell(std::shared_ptr<WFCPosition> position, unsigned long collapseTo)
{
	Enqueue(position, std::optional<unsigned long>(collapseTo));

	while (updateQueue.getCount() > 0) {
		//Loop without caring
	}

	std::vector temp = std::vector<WFCPosition>(dirtyPositions);
	dirtyPositions = std::vector<WFCPosition>();
	return temp;
}

void IWFCCollapseMethod::RegisterForCellUpdates(WFCPosition positionOfInterest, WFCCell toRegister)
{
	std::set<std::tuple<WFCPosition, std::vector<WFCCell>>>::iterator it;
	for (it = cellUpdates.begin(); it != cellUpdates.end(); ++it) {
		//std::get<0>(it);
	}
	//cellUpdates.find()
}

void IWFCCollapseMethod::DeRegisterForCellUpdates(WFCPosition positionOfInterest, WFCCell toDeregister)
{
}

void IWFCCollapseMethod::Reset()
{
}


