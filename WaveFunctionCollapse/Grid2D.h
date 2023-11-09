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
        > entropyQueue;

    //2d vector of a list of shared pointers
    std::vector<std::vector<std::vector<std::shared_ptr<WFCCell>>>> cellsToUpdate;
public:
    Grid2D(WFCPosition& newSize);

    virtual int RemainingCellsToCollapse() override;
    virtual std::shared_ptr<WFCPosition> PopNextCellToCollapse() override;
    virtual std::shared_ptr<WFCCell> GetCell(std::shared_ptr <WFCPosition> position) override;
    virtual std::vector<std::shared_ptr<WFCCell>> GetAlertees(WFCPosition& positionOfInterest) override;
    virtual void RegisterForCellUpdates(std::shared_ptr<WFCPosition> positionOfInterest, std::shared_ptr<WFCCell> toRegister) override;
    virtual void DeRegisterForCellUpdates(std::shared_ptr<WFCPosition> positionOfInterest, std::shared_ptr <WFCCell> toDeregister) override;
};

