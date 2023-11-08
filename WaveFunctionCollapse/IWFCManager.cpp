#pragma once
#include "IWFCManager.h"
#include "IWFCCollapseMethod.h"
#include "IWFCGrid.h"
#include "WFCCell.h"



IWFCManager::IWFCManager(IWFCCollapseMethod* collapse, IWFCGrid* grid) : _collapseMethod(collapse), _grid(grid)
{
}

inline IWFCManager::~IWFCManager()
{
}

void IWFCManager::GenerateOnce()
{
	//Grid.SortQueue
	//Collapse
}

void IWFCManager::Collapse()
{
	//CollapseMethod.Collapse(Grid.PopNextCellToCollapse());
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
