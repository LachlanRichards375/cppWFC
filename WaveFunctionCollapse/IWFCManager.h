#pragma once
#include <memory>
#include "WFCposition.h"
#include <vector>
struct WFCCellUpdate;
class IWFCGrid;
class IWFCCollapseMethod;
class WFCCell;

class IWFCManager
{
protected:
	void GenerateOnce();
	void Collapse();

	IWFCGrid* _grid;
	IWFCCollapseMethod* _collapseMethod;

public:
	IWFCManager(IWFCCollapseMethod* collapse, IWFCGrid* grid);
	~IWFCManager();

	void PrintGrid();

	void CollapseSpecificCell(WFCPosition* position, unsigned long toCollapseTo);
	WFCCell* GetCell(WFCPosition* position);
	WFCPosition& GetGridSize();
	void RegisterForAlert(WFCPosition* position, WFCCell* alertee);
	std::vector<WFCCell*> GetAlertees(const WFCPosition* position);
	void Generate();
};