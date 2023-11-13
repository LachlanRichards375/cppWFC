#pragma once
#include "IWFCManager.h"
#include "IWFCCollapseMethod.h"
#include "IWFCGrid.h"
#include "WFCCell.h"
#include <iostream>


IWFCManager::IWFCManager(IWFCCollapseMethod* collapse, IWFCGrid* grid) : _collapseMethod(collapse), _grid(grid)
{
	//Rule Setup
}

IWFCManager::~IWFCManager()
{
}

void IWFCManager::GenerateOnce()
{
	//Grid.SortQueue
	//Collapse
}

void IWFCManager::Collapse()
{
	_collapseMethod->Collapse(_grid->PopNextCellToCollapse());
}

void IWFCManager::CollapseSpecificCell(std::shared_ptr <WFCPosition> position, unsigned long toCollapseTo)
{
	//CollapseMethod.CollapseSpecificCell(position, toCollapseTo);
}

std::shared_ptr<WFCCell> IWFCManager::GetCell(std::shared_ptr <WFCPosition> position)
{
	return _grid->GetCell(position);
}

WFCPosition& IWFCManager::GetGridSize()
{
	return _grid->GetSize();
}

std::vector<std::shared_ptr<WFCCell>> IWFCManager::GetAlertees(WFCPosition& position)
{
	return _grid->GetAlertees(position);
}

void IWFCManager::Initialize()
{
	_collapseMethod->SetManager(this);
	//Set max thread count

	//Grid Initalize?
	//Grid Rule Setup
	_grid->RuleSetup();
}

void IWFCManager::Generate()
{
	std::cout << "In Generate" << std::endl;
	while (_grid->RemainingCellsToCollapse() > 0) {
		GenerateOnce();
	}
}
