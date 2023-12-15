#include "pch.h"
#include "CppUnitTest.h"
#include "Grid2D.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(Grid2dTests)
	{
	public:
		TEST_METHOD(Constructor)
		{
			//Do stuff
			Assert::IsTrue(true);
			WFCPosition expectedSize{ 5,5 };
			Grid2D grid{ expectedSize };

			Assert::AreEqual(grid.GetSize(), expectedSize);
			Assert::IsNull(grid.GetCell(new WFCPosition(2, 2)));
		}
	};
}