#pragma once
#include "IWFCManager.h"
#include "IWFCCollapseMethod.h"
#include "IWFCGrid.h"
#include "WFCCell.h"
#include <iostream>
#include "../tracy/public/tracy/Tracy.hpp"


IWFCManager::IWFCManager(IWFCCollapseMethod* collapse, IWFCGrid* grid, short threadCount) : _collapseMethod(collapse), _grid(grid), _threadPool(ThreadPool())
{
	{
		ZoneScopedN("Starting Thread pool");
		_threadPool.Start(threadCount);
	}
	_collapseMethod->Initialize(this);
	_grid->Initialize(this);
	
}

IWFCManager::~IWFCManager()
{
	_threadPool.Stop();
}

void IWFCManager::GenerateOnce()
{
	ZoneScopedN("GenerateOnce");
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

void IWFCManager::QueueJobToThreadPool(const std::function<void()>& job)
{
	_threadPool.QueueJob(job);
}

bool IWFCManager::IsThreadPoolBusy()
{
	return _threadPool.busy();
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
	#ifdef _DEBUG
		std::cout << std::endl << "---------------- DIVIDER ----------------" << std::endl;
		PrintGrid();
		while (_grid->RemainingCellsToCollapse() > 0) {
			std::cout << std::endl << "---------------- DIVIDER ----------------" << std::endl;
			GenerateOnce();
			std::cout << std::endl << "---------------- DIVIDER ----------------" << std::endl;
			PrintGrid();
		}
	#else
		while (_grid->RemainingCellsToCollapse() > 0) {
			GenerateOnce();
		}
	#endif
}

void IWFCManager::PrintGrid() {
	_grid->PrintGrid();
}
