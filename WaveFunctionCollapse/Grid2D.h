#pragma once
#include "IWFCGrid.h"
#include <memory>

class Grid2D :
    public IWFCGrid
{
protected:
    std::vector<std::vector<std::shared_ptr<WFCCell>>> grid;
    //2d vector of a list of shared pointers
    std::vector<std::vector<std::vector<std::shared_ptr<WFCCell>>>> cellsToUpdate;
public:
    Grid2D(WFCPosition& newSize);

    virtual void RuleSetup() override;
    virtual int RemainingCellsToCollapse() override;
    virtual std::shared_ptr<WFCCell> GetCell(std::shared_ptr <WFCPosition> position) override;
    virtual std::vector<std::shared_ptr<WFCCell>> GetAlertees(const WFCPosition& positionOfInterest) override;
    virtual void RegisterForCellUpdates(std::shared_ptr<WFCPosition> positionOfInterest, std::shared_ptr<WFCCell> toRegister) override;
    virtual void DeRegisterForCellUpdates(std::shared_ptr<WFCPosition> positionOfInterest, std::shared_ptr <WFCCell> toDeregister) override;
};

