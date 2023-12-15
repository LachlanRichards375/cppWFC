#include "pch.h"
#include "CppUnitTest.h"
#include "IWFCCollapseMethod.h"
#include "IWFCManager.h"
#include "Grid2D.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(IWFCCollapseMethodTests)
	{
	public:
		WFCPosition expectedSize{ 5,5 };
		Grid2D grid{ expectedSize };
		IWFCCollapseMethod collapse{ };
		IWFCManager manager{ &collapse, &grid, 1 };
		TEST_METHOD_INITIALIZE(initialize) {
			collapse.Initialize(&manager);
		}
		TEST_METHOD(EmptyTest) {
			Assert::IsTrue(true);
		}
	};
}