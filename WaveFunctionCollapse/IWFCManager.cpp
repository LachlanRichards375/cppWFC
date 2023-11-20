#pragma once
#include "IWFCManager.h"
#include "IWFCCollapseMethod.h"
#include "IWFCGrid.h"
#include "WFCCell.h"
#include <iostream>


IWFCManager::IWFCManager(IWFCCollapseMethod* collapse, IWFCGrid* grid) : _collapseMethod(collapse), _grid(grid)
{

	_collapseMethod->SetManager(this);
	_grid->Initialize(this);
	//Set max thread count
}

IWFCManager::~IWFCManager()
{
}

void IWFCManager::GenerateOnce()
{
	//std::cout << "Generating Once" << std::endl;
	_grid->SortQueue();
	Collapse();
}

void IWFCManager::Collapse()
{
	WFCCell* cellToCollapse{ _grid->PopNextCellToCollapse() };
	//std::cout << cellToCollapse->GetPosition()->x << "," << cellToCollapse->GetPosition()->y << std::endl;
	_collapseMethod->Collapse(cellToCollapse);
}

void IWFCManager::CollapseSpecificCell(std::shared_ptr <WFCPosition> position, unsigned long toCollapseTo)
{
	//CollapseMethod.CollapseSpecificCell(position, toCollapseTo);
}

WFCCell* IWFCManager::GetCell(std::shared_ptr <WFCPosition> position)
{
	return _grid->GetCell(position);
}

WFCPosition& IWFCManager::GetGridSize()
{
	return _grid->GetSize();
}

std::vector<WFCCell*> IWFCManager::GetAlertees(const WFCPosition* position)
{
	return _grid->GetAlertees(position);
}

void IWFCManager::RegisterForAlert(std::shared_ptr<WFCPosition> registerFor, WFCCell* alertee) {
	_grid->RegisterForCellUpdates(registerFor, alertee);
}

void IWFCManager::Generate()
{
	std::cout << "In Generate, cells to collapse:" << _grid->RemainingCellsToCollapse() << std::endl;
	while (_grid->RemainingCellsToCollapse() > 0) {
		GenerateOnce();
	}
}

void IWFCManager::PrintGrid() {
	_grid->PrintGrid();
}
