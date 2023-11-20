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
	void CollapseSpecificCell(std::shared_ptr <WFCPosition> position, unsigned long toCollapseTo);

	IWFCGrid* _grid;
	IWFCCollapseMethod* _collapseMethod;

public:
	IWFCManager(IWFCCollapseMethod* collapse, IWFCGrid* grid);
	~IWFCManager();

	void PrintGrid();

	WFCCell* GetCell(std::shared_ptr<WFCPosition> position);
	WFCPosition& GetGridSize();
	void RegisterForAlert(WFCPosition* position, WFCCell* alertee);
	std::vector<WFCCell*> GetAlertees(const WFCPosition* position);
	void Generate();
};