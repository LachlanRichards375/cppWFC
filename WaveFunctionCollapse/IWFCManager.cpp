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
		IWFCManager::threadCount = threadCount;
		_threadPool.Start(threadCount);
	}
	_collapseMethod->Initialize(this);
	_grid->Initialize(this);
	
}

IWFCManager::~IWFCManager()
{
	_threadPool.Stop();
}

void IWFCManager::Collapse()
{
	ZoneScopedN("Collapse()");
	{
		ZoneScopedN("Collapse");
		_collapseMethod->Collapse(_grid->PopNextCellToCollapse());
	}
	{
		ZoneScopedN("Sorting Queue");
		_grid->SortQueue();
	}
}

void IWFCManager::CollapseSpecificCell(WFCPosition* position, unsigned long toCollapseTo)
{
	ZoneScopedN("Collapse(specific)");
	{
		ZoneScopedN("Collapse");
		_collapseMethod->CollapseSpecificCell(_grid->PopSpecificCell(position, GetCell(position)->CalculateEntropy()), toCollapseTo);
	}
	{
		ZoneScopedN("Sorting Queue");
		_grid->SortQueue();
	}
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
			Collapse();
			std::cout << std::endl << "---------------- DIVIDER ----------------" << std::endl;
			std::cout << "Cells remaining: " << _grid->RemainingCellsToCollapse() << std::endl;
			PrintGrid();
		}
	#else
		while (_grid->RemainingCellsToCollapse() > 0) {
			Collapse();
		}
	#endif
}

void IWFCManager::MarkDirty(unsigned long oldDomainCount, int index)
{
	_grid->MarkDirty(oldDomainCount, index);
}

void IWFCManager::PrintGrid() {
	_grid->PrintGrid();
}


void IWFCManager::Reset() {
	//clear the thread pool
	_threadPool.Stop();
	_threadPool.Start(threadCount);
	_collapseMethod->Reset();
	_grid->Reset();
}

void IWFCManager::GenerateOnce()
{
	Collapse();
}
