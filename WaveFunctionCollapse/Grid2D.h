#pragma once
#include "IWFCGrid.h"
#include <memory>

class Grid2D :
    public IWFCGrid
{
protected:
    std::vector<std::vector<WFCCell*>> grid;
    //2d vector of a list of shared pointers
    std::vector<std::vector<std::vector<WFCCell*>>> cellsToUpdate;
public:
    Grid2D(WFCPosition& newSize);
    virtual void Initialize(IWFCManager* manager) override;

    virtual WFCCell* GetCell(std::shared_ptr <WFCPosition> position) override;
    virtual std::vector<WFCCell*> GetAlertees(const WFCPosition& positionOfInterest) override;
    virtual void RegisterForCellUpdates(std::shared_ptr<WFCPosition> positionOfInterest, WFCCell* toRegister) override;
    virtual void DeRegisterForCellUpdates(std::shared_ptr<WFCPosition> positionOfInterest, WFCCell* toDeregister) override;
    virtual void PrintGrid() override;
};


