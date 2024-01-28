#include "Grid2D.h"
#include "WFCCell.h"
#include <iostream>
#include "WFCRuleManager.h"
#include "SortedVector.h"
#include <string>
#include "../tracy/public/tracy/Tracy.hpp"

Grid2D::Grid2D(WFCPosition& newSize)
	:IWFCGrid(newSize)
{
	
}
#pragma optimize("", off)
void Grid2D::WaitForRulesToSetup() {
	while (ruleSetupsFinished > 0) {

	}
}
#pragma optimize("", on)

void Grid2D::Initialize(IWFCManager* newManager) {
	ZoneScopedN("Grid2D Initialize");
	//Set manager
	IWFCGrid::manager = newManager;
	initializing = true;
	{
		ZoneScopedN("Resizing grid and cells to update");
		//Set grid size
		grid.resize(size.x, std::vector<WFCCell*>(size.y));
		//Initialize cells to update list
		cellsToUpdate.resize(size.x, std::vector<std::unordered_set<WFCCell*>>(size.y));
	}

	{
		ZoneScopedN("Print size of grid");
		std::string output = "Creating grid of size: ";
		output.append(std::to_string(size.x).append(",").append(std::to_string(size.y)));
		std::cout << output << std::endl;
	}

	{
		ZoneScopedN("Filling the cells");
		ruleSetupsFinished = 0;
		for (int x = 0; x < size.x; ++x) {
			for (int y = 0; y < size.y; ++y) {
				WFCCell* cell = new WFCCell(manager, new WFCPosition{ x, y }, WFCRuleManager::GetInitialDomain());
				grid[x][y] = cell;
				cellsToSetup.enqueue(cell);
				entropyQueue.insert(cell);
				++ruleSetupsFinished;
			}
		}
		{
			ZoneScopedN("Queueing jobs");
			//Start threads to deal with the rules
			for (int i = 0; i < manager->NumThreads; i++) {
				buffers.push_back(std::vector<BufferNotification>());
			}
			for (int i = 0; i < manager->NumThreads; i++) {
				int localIndex = i;
				manager->QueueJobToThreadPool([this, localIndex] {RuleSetupJob(localIndex); });
			}
		}
		{
			ZoneScopedN("Waiting for jobs to finish");
			//Wait for rules to setup
			WaitForRulesToSetup();
		}
		{
			ZoneScopedN("Flushing buffers");
			//flush buffer
			for (auto buffer : buffers) {
				for (BufferNotification bufferNotification : buffer) {
					cellsToUpdate[bufferNotification.positionOfInterest->x][bufferNotification.positionOfInterest->y].insert(bufferNotification.toRegister);
				}
			}
		}
		initializing = false;
	}
}

void Grid2D::RuleSetupJob(int bufferIndex) {
	ZoneScopedN("RuleSetupThread");
	{
		//Commender the JobCountMutex to insert buffers
		std::unique_lock<std::mutex> BufferIndexMapInsertion(job_count_mutex);
		bufferIndexMap.insert({ std::this_thread::get_id(),bufferIndex });
	}
	while (cellsToSetup.getCount() > 0) {
		ZoneScopedN("Cell.Setup()");
		if (cellsToSetup.getCount() == 0) {
			continue;
		}
		WFCCell* cell = cellsToSetup.dequeue();
		cell->RuleSetup();
		{
			std::unique_lock<std::mutex> NumRuleSetupFinishedLock(job_count_mutex);
			--ruleSetupsFinished;
		}
	}
}

inline WFCCell* Grid2D::GetCell(WFCPosition* position) {
	return grid[position->x][position->y];
}

inline std::unordered_set<WFCCell*> Grid2D::GetAlertees(const WFCPosition* positionOfInterest) {
	return cellsToUpdate[positionOfInterest->x][positionOfInterest->y];
}

void Grid2D::RegisterForCellUpdates(WFCPosition* positionOfInterest, WFCCell* toRegister)
{
	ZoneScopedN("RegisterForCellUpdates");
	if (positionOfInterest->x < 0 || positionOfInterest->x >= size.x) {
		return;
	}
	if (positionOfInterest->y < 0 || positionOfInterest->y >= size.y) {
		return;
	}

	if (initializing) {
		//if initializing we have the buffer threads active
		int index = bufferIndexMap[std::this_thread::get_id()];
		//std::cout << "\nwriting into buffer index: " << index;
		BufferNotification toPushBack ;
		WFCPosition* test{ positionOfInterest };
		WFCCell* cellTest{ toRegister };
		buffers[index].push_back({test, cellTest});
	}
	else {
		cellsToUpdate[positionOfInterest->x][positionOfInterest->y].insert(toRegister);
	}
}

void Grid2D::DeRegisterForCellUpdates(WFCPosition* positionOfInterest, WFCCell* toDeregister)
{
	if (positionOfInterest->x < 0 || positionOfInterest->x >= size.x) {
		return;
	}
	if (positionOfInterest->y < 0 || positionOfInterest->y >= size.y) {
		return;
	}
	cellsToUpdate[positionOfInterest->x][positionOfInterest->y].erase(toDeregister);
}

void Grid2D::PrintGrid()
{
	std::cout << "Printing 2D grid: " << std::endl;
	for(int row = size.x-1; row>=0;--row){ //0 bottom left
	//for (int x = 0; x < size.x; ++x) { //0 top left
		std::string output = "|";
		for (int col = 0; col < size.y; ++col) {
			unsigned long long collapsedTile = grid[col][row]->CollapsedTile;

			if (collapsedTile < 100) {
				output.append(" ");
			}

			output.append(std::to_string(collapsedTile));

			if (collapsedTile < 10) {
				output.append(" ");
			}
		}
		output.append("|");
		std::cout << output << std::endl;
	}
}

void Grid2D::Reset() {
	//Set grid size
	grid.clear();
	grid.resize(size.x, std::vector<WFCCell*>(size.y));
	//clear cells to update list
	cellsToUpdate.clear();
	cellsToUpdate.resize(size.x, std::vector<std::unordered_set<WFCCell*>>(size.y));
	
	entropyQueue.clear();

	std::string output = "Creating grid of size: ";
	output.append(std::to_string(size.x).append(",").append(std::to_string(size.y)));
	std::cout << output << std::endl;
	for (int x = 0; x < size.x; ++x) {
		for (int y = 0; y < size.y; ++y) {
			WFCCell* cell = new WFCCell(manager, new WFCPosition{ x, y }, WFCRuleManager::GetInitialDomain());
			grid[x][y] = cell;
			cell->RuleSetup();
			entropyQueue.insert(cell);
		}
	}
}


std::vector<unsigned long long > exporter;
std::vector<unsigned long long>* Grid2D::Export() {
	exporter.clear();
	for (auto& row : grid) {
		for (auto& column : row) {
			exporter.push_back(column->CollapsedTile);
		}
	}

	return &exporter;
}
