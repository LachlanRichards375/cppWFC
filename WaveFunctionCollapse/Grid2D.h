#pragma once
#include "IWFCGrid.h"
#include <memory>
#include <queue>

class Grid2D :
    public IWFCGrid
{
protected:
    std::vector<std::vector<std::shared_ptr<WFCCell>>> grid;
    std::priority_queue<
        std::shared_ptr<WFCCell>, //Type
        std::vector<std::shared_ptr<WFCCell>>, //Container
        std::greater<WFCCell> //Makes it sort min priority
        > entorpyQueue;
public:
    virtual std::shared_ptr<WFCPosition> PopNextCellToCollapse() override;
    virtual std::shared_ptr<WFCCell> GetCell(std::shared_ptr <WFCPosition> position) override;
};

