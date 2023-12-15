#include "pch.h"
#include "CppUnitTest.h"
#include "Grid2D.h"
#include "WFCPosition.h"
#include "IWFCManager.h"
#include "IWFCCollapseMethod.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(Grid2dTests)
	{
	public:
		WFCPosition expectedSize{ 5,5 };
		Grid2D grid{ expectedSize };
		IWFCCollapseMethod collapse{ };
		IWFCManager manager{ &collapse, &grid, 1 };
		TEST_METHOD(Constructor)
		{
			//Do stuff
			Assert::IsTrue(true);
			Grid2D constructorGrid{ expectedSize };

			Assert::AreEqual(constructorGrid.GetSize().x, expectedSize.x);
			Assert::AreEqual(constructorGrid.GetSize().y, expectedSize.y);
		}
		TEST_METHOD_INITIALIZE(initialize) {
			grid.Initialize(&manager);
		}
		TEST_METHOD(PostInitialize){
			Assert::IsNotNull(grid.GetCell(new WFCPosition(2, 2)));
		}
	};
}