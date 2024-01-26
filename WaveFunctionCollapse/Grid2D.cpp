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

void Grid2D::Initialize(IWFCManager* newManager) {
	ZoneScopedN("Grid2D Initialize");
	//Set manager
	IWFCGrid::manager = newManager;

	{
		ZoneScopedN("Resizing grid and cells to update");
		//Set grid size
		grid.resize(size.x, std::vector<WFCCell*>(size.y));
		//Initialize cells to update list
		cellsToUpdate.resize(size.x, std::vector<std::unordered_set<WFCCell*>>(size.y));
		threadCellsToUpdateBufferCopy = cellsToUpdate;
	}

	{
		ZoneScopedN("Print size of grid");
		std::string output = "Creating grid of size: ";
		output.append(std::to_string(size.x).append(",").append(std::to_string(size.y)));
		std::cout << output << std::endl;
	}

	{
		ZoneScopedN("Filling the cells");
		//Start Buffer threads
		for (int i = 0; i < manager->NumThreads; ++i) {
			manager->QueueJobToThreadPool([this] {RegisterForCellUpdateBuffer(); });
		}
		//Setup each cell
		numInBuffer = 0;
		for (int x = 0; x < size.x; ++x) {
			for (int y = 0; y < size.y; ++y) {
				WFCCell* cell;
				{
					ZoneScopedN("Create New Cell");
					cell = new WFCCell(manager, new WFCPosition{ x, y }, WFCRuleManager::GetInitialDomain());
				}
				grid[x][y] = cell;
				{
					ZoneScopedN("Rule Setup");
					cell->RuleSetup();
				}
				entropyQueue.insert(cell);
			}
		}
		//Wait for buffer to fill then release them
		waitForBufferToFill();
		flushBuffer = true;
		c.notify_all(); //Wake up threads there's monsters in the hallways

	}
}

inline WFCCell* Grid2D::GetCell(WFCPosition* position) {
	return grid[position->x][position->y];
}

inline std::unordered_set<WFCCell*> Grid2D::GetAlertees(const WFCPosition* positionOfInterest) {
	return cellsToUpdate[positionOfInterest->x][positionOfInterest->y];
}

#pragma optimize("", off)
void Grid2D::waitForBufferToFill()
{
	ZoneScopedN("Filling the cells");
	while (numInBuffer < (size.x * size.y)-1) {

	}
}
#pragma optimize("", on)

//This is thread loop
//RegisterForCellUpdates will notify when they have added a cell into the ThreadSafeQueue
//Dequeue and add to the buffer
//When FlushBuffer() is set to true we continue trying to add our elements to the regular queue with a lock.
void Grid2D::RegisterForCellUpdateBuffer() {
	//std::vector<std::vector<std::unordered_set<WFCCell*>>> buffer = {threadCellsToUpdateBufferCopy};
	std::vector<BufferNotification> bufferNotifications;
	while (!flushBuffer) {
		//Wait notification
		//Dequeue and add to buffer

		std::unique_lock<std::mutex> lock(m);

		while (toBeAddedToBuffer.getCount() <= 0)
		{
			// release lock as long as the wait and reaquire it afterwards.
			c.wait(lock);
		}

		if (flushBuffer) {
			break;
		}
		/*
		auto bN = toBeAddedToBuffer.dequeue();
		buffer[bN.positionOfInterest->x][bN.positionOfInterest->y].insert(bN.toRegister);
		*/

		bufferNotifications.push_back(toBeAddedToBuffer.dequeue());
		++numInBuffer;
	}
	//Lock mutex
	//Add our buffer to main
	//Release mutex

	std::lock_guard<std::mutex> lock(m);
	for (auto& current : bufferNotifications) {
		cellsToUpdate[current.positionOfInterest->x][current.positionOfInterest->y].insert(current.toRegister);
	}
}

//Register on the local thread
void Grid2D::RegisterForCellUpdates(WFCPosition* positionOfInterest, WFCCell* toRegister)
{
	ZoneScopedN("RegisterForCellUpdates");
	if (positionOfInterest->x < 0 || positionOfInterest->x >= size.x) {
		if (positionOfInterest->y < 0 || positionOfInterest->y >= size.y) {
			return;
		}
	}

	if (flushBuffer) {
		cellsToUpdate[positionOfInterest->x][positionOfInterest->y].insert(toRegister);
	}
	else {
		BufferNotification bN = { positionOfInterest, toRegister };
		toBeAddedToBuffer.enqueue(bN);
		c.notify_one();
	}
}

//Register on the main thread

void Grid2D::DeRegisterForCellUpdates(WFCPosition* positionOfInterest, WFCCell* toDeregister)
{
	if (positionOfInterest->x < 0 || positionOfInterest->x >= size.x) {
		if (positionOfInterest->y < 0 || positionOfInterest->y >= size.y) {
			return;
		}
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
