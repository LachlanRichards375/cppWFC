#include "pch.h"
#include "CppUnitTest.h"
#include "IWFCManager.h"
#include "IWFCCollapseMethod.h"
#include "Grid2D.h"
#include "HelperFunctions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{

	const unsigned long EMPTY = 0;
	const unsigned long GRASS = 1 << 0;
	const unsigned long SAND = 1 << 1;
	const unsigned long WATER = 1 << 2;

	TEST_CLASS(IntergrationTests)
	{
	public:
		TEST_CLASS_INITIALIZE(Initialize) {
			createRules();
			AddToDomain(GRASS + SAND + WATER);
		}
		TEST_METHOD(TestDomainByCollapseSpecific)
		{
			WFCPosition expectedSize{ 5,5 };
			Grid2D grid{ expectedSize };
			IWFCCollapseMethod collapse{ };
			IWFCManager manager{ &collapse, &grid, 1 };

			//Check cell is initially EMPTY
			WFCPosition* targetCell = new WFCPosition(2, 2);
			Assert::IsTrue((manager.GetCell(targetCell)->CollapsedTile == EMPTY), L"targetCell is not EMPTY");

			//Collapse middle cell to SAND
			manager.CollapseSpecificCell(targetCell, SAND);
			LogDomainGrid(grid, expectedSize);

			//Check middle cell is SAND
			Assert::IsTrue((manager.GetCell(targetCell)->CollapsedTile == SAND), L"targetCell is not SAND");

			//Check left cells can only be SAND or GRASS
			Assert::IsTrue(manager.GetCell(new WFCPosition(1, 3))->GetDomain() == (SAND | GRASS), L"Cell at (-1, 1) is not just SAND and GRASS");
			Assert::IsTrue(manager.GetCell(new WFCPosition(1, 2))->GetDomain() == (SAND | GRASS), L"Cell at (-1, 0) is not just SAND and GRASS");
			Assert::IsTrue(manager.GetCell(new WFCPosition(1, 1))->GetDomain() == (SAND | GRASS), L"Cell at (-1,-1) is not just SAND and GRASS");

			//Check right cells can only be SAND or WATER
			Assert::IsTrue(manager.GetCell(new WFCPosition(3, 3))->GetDomain() == (SAND | WATER), L"Cell at (1, 1) is not just SAND and WATER");
			Assert::IsTrue(manager.GetCell(new WFCPosition(3, 2))->GetDomain() == (SAND | WATER), L"Cell at (1, 0) is not just SAND and WATER");
			Assert::IsTrue(manager.GetCell(new WFCPosition(3, 1))->GetDomain() == (SAND | WATER), L"Cell at (1,-1) is not just SAND and WATER");


			//\\ Second Cell //\\


			WFCPosition* secondTargetCell = new WFCPosition(2, 1);
			//Collapse second cell to WATER
			manager.CollapseSpecificCell(secondTargetCell, WATER);
			LogDomainGrid(grid, expectedSize);

			//Check second cell is WATER
			Assert::IsTrue((manager.GetCell(secondTargetCell)->CollapsedTile == WATER), L"Second Collapse is not WATER");


			//Check left cells can only be SAND or GRASS
			Assert::IsTrue(manager.GetCell(new WFCPosition(1, 3))->GetDomain() == (SAND | GRASS), L"Cell at (-1, 1) is not just SAND and GRASS");
			Assert::IsTrue(manager.GetCell(new WFCPosition(1, 2))->GetDomain() == (SAND), L"Cell at (-1, 0) is not just SAND");
			Assert::IsTrue(manager.GetCell(new WFCPosition(1, 1))->GetDomain() == (SAND), L"Cell at (-1,-1) is not just SAND");

			//Bottom left can't be GRASS as we collapsed to water
			Assert::IsTrue(manager.GetCell(new WFCPosition(1, 0))->GetDomain() == (SAND | WATER), L"Cell at (-1,-1) is not just SAND and WATER");
			Assert::IsTrue(manager.GetCell(new WFCPosition(2, 0))->GetDomain() == (SAND | WATER), L"Cell at (-1,-1) is not just SAND and WATER");

			//Check right cells can only be SAND or WATER
			Assert::IsTrue(manager.GetCell(new WFCPosition(3, 3))->GetDomain() == (SAND | WATER), L"Cell at (1, 1) is not just SAND and WATER");
			Assert::IsTrue(manager.GetCell(new WFCPosition(3, 2))->GetDomain() == (WATER), L"Cell at (1, 0) is not just WATER");
			Assert::IsTrue(manager.GetCell(new WFCPosition(3, 1))->GetDomain() == (WATER), L"Cell at (1,-1) is not just WATER");
			Assert::IsTrue(manager.GetCell(new WFCPosition(3, 0))->GetDomain() == (WATER), L"Cell at (1,-1) is not just WATER");
		}

		TEST_METHOD(TestCollapseOrder) {

			WFCPosition expectedSize{ 3,3 };
			Grid2D grid{ expectedSize };
			IWFCCollapseMethod collapse{ };
			IWFCManager manager{ &collapse, &grid, 1 };

			//Check cell is initially EMPTY
			WFCPosition* targetCell = new WFCPosition(1, 1);
			Assert::IsTrue((manager.GetCell(targetCell)->CollapsedTile == EMPTY), L"targetCell is not EMPTY");

			//Collapse middle cell to SAND
			manager.CollapseSpecificCell(targetCell, SAND);
			LogEntropyGrid(grid, expectedSize);
			//Check middle cell is SAND
			Assert::IsTrue((manager.GetCell(targetCell)->CollapsedTile == SAND), L"targetCell is not SAND");

			std::vector<const WFCPosition*> lowestEntropy;
			bool foundLowestEntropy;
			while (grid.RemainingCellsToCollapse() > 0) {
				foundLowestEntropy = false;
				lowestEntropy = manager.GetLowestEntropy();
				manager.GenerateOnce();

				for (auto& it : lowestEntropy) {
					//If it returns 0 it means it collapsed
					if (manager.GetCell(const_cast<WFCPosition*>(it))->CalculateEntropy() == 0) {
						foundLowestEntropy = true;
					}
				}
				Assert::IsTrue(foundLowestEntropy, L"Collapse did not act on a cell in the lowest entropy");
				LogEntropyGrid(grid, expectedSize);
			}
		}
	};
}
