#pragma once

#include "pch.h"
#include "CppUnitTest.h"
#include "IWFCManager.h"
#include "Grid2D.h"
namespace UnitTests
{
	void LogCollapsedGrid(Grid2D grid, WFCPosition expectedSize);
	void LogEntropyGrid(Grid2D grid, WFCPosition expectedSize);
	void LogDomainGrid(Grid2D grid, WFCPosition expectedSize); 
	void createRules();
	void AddToDomain(unsigned long long tilesToAdd);
}