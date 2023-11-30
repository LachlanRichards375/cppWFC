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
	//std::cout << cellToCollapse->GetPosition()->x << "," << cellToCollapse->GetPosition()->y << std::endl;
	_collapseMethod->Collapse(_grid->PopNextCellToCollapse());
}

void IWFCManager::CollapseSpecificCell(WFCPosition* position, unsigned long toCollapseTo)
{
	_collapseMethod->CollapseSpecificCell(_grid->PopSpecificCell(position), toCollapseTo);
}

WFCCell* IWFCManager::GetCell(WFCPosition* position)
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

void IWFCManager::RegisterForAlert(WFCPosition* registerFor, WFCCell* alertee) {
	_grid->RegisterForCellUpdates(registerFor, alertee);
}

void IWFCManager::Generate()
{
	std::cout << "In Generate, cells to collapse:" << _grid->RemainingCellsToCollapse() << std::endl;
	std::cout << std::endl << "---------------- DIVIDER ----------------" << std::endl;
	PrintGrid();
	while (_grid->RemainingCellsToCollapse() > 0) {
		std::cout << std::endl << "---------------- DIVIDER ----------------" << std::endl;
		GenerateOnce();
		std::cout << std::endl << "---------------- DIVIDER ----------------" << std::endl;
		PrintGrid();
	}
}

void IWFCManager::PrintGrid() {
	_grid->PrintGrid();
}
