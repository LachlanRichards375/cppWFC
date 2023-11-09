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
}

void IWFCManager::Generate()
{
	std::cout << "In Generate";
	while (_grid->RemainingCellsToCollapse() > 0) {
		Collapse();
	}
}
