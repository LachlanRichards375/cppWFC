#include "pch.h"
#include "CppUnitTest.h"
#include "IWFCManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(IWFCManagerTests)
	{
	public:

		TEST_METHOD(TestMethod1)
		{
			//Do stuff
			Assert::IsTrue(true);
		}
	};
}

/*

void createRules() {
	ZoneScopedN("Creating WFCRules");
	//1 Grass
	//2 Sand
	//4 Water
	WFCPosition IgnoreWest[]{ new WFCPosition(-1,0), new WFCPosition(-1,1), new WFCPosition(-1,-1) };
	WFCPosition IgnoreEast[]{ new WFCPosition(1,0), new WFCPosition(1,1), new WFCPosition(1,-1) };
	WFCPosition IgnoreAllAround[]{  new WFCPosition(-1,1), new WFCPosition(0,1), new WFCPosition(1,1),
									new WFCPosition(-1,0),		   TILE			 new WFCPosition(1, 0),
									new WFCPosition(-1, -1), new WFCPosition(0, -1), new WFCPosition(1, -1), };
	{
		ZoneScopedN("Adding Grass Rules");
		//Grass cant have Sand West
		WFCRule_Add_CellIsNot(GRASS, SAND, 3, IgnoreWest);
		//Grass can't have Water anywhere
		WFCRule_Add_CellIsNot(GRASS, WATER, 8, IgnoreAllAround);
	}
	{
		ZoneScopedN("Adding Sand Rules");
		//Sand can't have Water West
		WFCRule_Add_CellIsNot(SAND, WATER, 3, IgnoreWest);
		//Sand can't have Grass East
		WFCRule_Add_CellIsNot(SAND, GRASS, 3, IgnoreEast);
	}
	{
		ZoneScopedN("Adding Water Rules");
		//Water cant have Sand East
		WFCRule_Add_CellIsNot(WATER, SAND, 3, IgnoreEast);
		//Water can't have Grass anywhere
		WFCRule_Add_CellIsNot(WATER, GRASS, 8, IgnoreAllAround);
	}

	{
		ZoneScopedN("Printing tile id's to screen");
		std::string output = "Cells: ";
		output.append("\n\t| -GRASS ->").append(std::ToString(GRASS))
			.append("\n\t|- SAND  ->").append(std::ToString(SAND))
			.append("\n\t|- WATER ->").append(std::ToString(WATER));
		std::cout << output << std::endl;
	}
}

int main(int argc, char* argv[]) {
	IWFCManager* manager;
	int messageNo = 0;
	auto t1 = std::chrono::high_resolution_clock::now();

	{
		ZoneScopedN("Generate Result");
		WFCPosition* size = new WFCPosition(10, 10);
		IWFCCollapseMethod* collapse;
		IWFCGrid* grid;

		{
			ZoneScopedN("Initialize");

			AddTileToDomain(SAND + WATER + GRASS);
			createRules();

			{
				ZoneScopedN("CollapseMethod Create");
				collapse = Threaded2DCollapse_Create();
			}
			{
				ZoneScopedN("Grid2D Create");
				grid = Grid2D_Create(size);
			}
			{
				ZoneScopedN("Manager Create");
				manager = IWFCManager_Create(collapse, grid, 12);
			}
		}

		{
			ZoneScopedN("ForceCollapse sand");
			messageNo = IWFCManager_Collapse(manager, SAND, new WFCPosition(size->x / 2, size->y / 2));
			if (messageNo != 0) {
				std::cout << "Error (" << messageNo << ") collapsing first cell to Sand. Program Aborting." << std::endl;
				return 0;
			}
		}

		{
			ZoneScopedN("Generate");
			messageNo = IWFCManager_Run(manager);
			if (messageNo != 0) {
				std::cout << "Error Generating: (" << messageNo << ") program aborting." << std::endl;
			}
		}

	}
	{
		ZoneScopedN("Printing result");

		auto t2 = std::chrono::high_resolution_clock::now();

		//Getting number of milliseconds as a double.
		std::chrono::duration<double, std::milli> ms_double = t2 - t1;

		std::cout << ms_double.count() << "ms" << std::endl;
		
	}

	return 0;
}
*/