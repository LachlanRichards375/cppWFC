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

	std::shared_ptr<WFCCell> GetCell(std::shared_ptr<WFCPosition> position);
	WFCPosition& GetGridSize();

	std::vector<std::shared_ptr<WFCCell>> GetAlertees(const WFCPosition& position);
	void Generate();
};