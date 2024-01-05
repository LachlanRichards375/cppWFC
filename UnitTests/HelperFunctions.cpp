#include "pch.h"
#include "HelperFunctions.h"
#include "WFCRuleManager.h"
#include "WFCRuleCellIsNot.h"
#include "WFCExternRunner.cpp"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace UnitTests
{
	void LogCollapsedGrid(Grid2D grid, WFCPosition expectedSize) {
		Logger::WriteMessage("Log Collapsed Grid: \n");
		for (int row = expectedSize.x - 1; row >= 0; --row) { //0 bottom left
			//for (int x = 0; x < size.x; ++x) { //0 top left
			std::string output = "|";
			for (int col = 0; col < expectedSize.y; ++col) {
				unsigned long long collapsedTile = grid.GetCell(new WFCPosition(col, row))->CollapsedTile;

				if (collapsedTile < 100) {
					output.append(" ");
				}

				output.append(std::to_string(collapsedTile));

				if (collapsedTile < 10) {
					output.append(" ");
				}
			}
			output.append("|\n");
			Logger::WriteMessage(output.c_str());
		}
		Logger::WriteMessage("\n");
	}

	void LogEntropyGrid(Grid2D grid, WFCPosition expectedSize) {
		Logger::WriteMessage("Log Entropy Grid: \n");
		for (int row = expectedSize.x - 1; row >= 0; --row) { //0 bottom left
			//for (int x = 0; x < size.x; ++x) { //0 top left
			std::string output = "|";
			for (int col = 0; col < expectedSize.y; ++col) {
				unsigned long long collapsedTile = grid.GetCell(new WFCPosition(col, row))->CalculateEntropy();

				if (collapsedTile < 100) {
					output.append(" ");
				}

				output.append(std::to_string(collapsedTile));

				if (collapsedTile < 10) {
					output.append(" ");
				}
			}
			output.append("|\n");
			Logger::WriteMessage(output.c_str());
		}
		Logger::WriteMessage("\n");
	}

	void LogDomainGrid(Grid2D grid, WFCPosition expectedSize) {
		Logger::WriteMessage("Log Domain Grid: \n");
		for (int row = expectedSize.x - 1; row >= 0; --row) { //0 bottom left
			//for (int x = 0; x < size.x; ++x) { //0 top left
			std::string output = "|";
			for (int col = 0; col < expectedSize.y; ++col) {
				unsigned long long collapsedTile = grid.GetCell(new WFCPosition(col, row))->GetDomain();

				if (collapsedTile < 100) {
					output.append(" ");
				}

				output.append(std::to_string(collapsedTile));

				if (collapsedTile < 10) {
					output.append(" ");
				}
			}
			output.append("|\n");
			Logger::WriteMessage(output.c_str());
		}
		Logger::WriteMessage("\n");
	}
	
	void AddToDomain(unsigned long long tilesToAdd)
	{
		//calls WFCExtern AddTileToDomain
		AddTileToDomain(tilesToAdd);
	}

	void createRules() {
		const unsigned long long EMPTY = 0;
		const unsigned long long GRASS = 1 << 0;
		const unsigned long long SAND = 1 << 1;
		const unsigned long long WATER = 1 << 2;
		WFCPosition IgnoreWest[]{ new WFCPosition(-1,0), new WFCPosition(-1,1), new WFCPosition(-1,-1) };
		WFCPosition IgnoreEast[]{ new WFCPosition(1,0), new WFCPosition(1,1), new WFCPosition(1,-1) };
		WFCPosition IgnoreAllAround[]{ new WFCPosition(-1,1), new WFCPosition(0,1), new WFCPosition(1,1),
										new WFCPosition(-1,0),	/*	   TILE		*/	 new WFCPosition(1,0),
										new WFCPosition(-1,-1),new WFCPosition(0,-1),new WFCPosition(1,-1), };
		//Grass cant have Sand West
		WFCRule_Add_CellIsNot(GRASS, SAND, 3, IgnoreWest);
		//Grass can't have Water anywhere
		WFCRule_Add_CellIsNot(GRASS, WATER, 8, IgnoreAllAround);


		//Sand can't have Water West
		WFCRule_Add_CellIsNot(SAND, WATER, 3, IgnoreWest);
		//Sand can't have Grass East
		WFCRule_Add_CellIsNot(SAND, GRASS, 3, IgnoreEast);


		//Water cant have Sand East
		WFCRule_Add_CellIsNot(WATER, SAND, 3, IgnoreEast);
		//Water can't have Grass anywhere
		WFCRule_Add_CellIsNot(WATER, GRASS, 8, IgnoreAllAround);
	}
}