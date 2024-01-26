#pragma once
#include "IWFCGrid.h"
#include <memory>
#include <unordered_set>
#include <mutex>
#include <condition_variable>
#include "ThreadSafeQueue.h"

struct BufferNotification {
public:
    WFCPosition* positionOfInterest;
    WFCCell* toRegister;
};

class Grid2D :
    public IWFCGrid
{
protected:
    std::vector<std::vector<WFCCell*>> grid;
    //2d vector of a list of pointers
    std::vector<std::vector<std::unordered_set<WFCCell*>>> cellsToUpdate;

    //Threaded Variables

    mutable std::mutex m;
    std::condition_variable c;
    bool flushBuffer;
    SafeQueue<BufferNotification> toBeAddedToBuffer;
    int numInBuffer;
    void waitForBufferToFill();
    std::vector<std::vector<std::unordered_set<WFCCell*>>> threadCellsToUpdateBufferCopy;

public:
    Grid2D(WFCPosition& newSize);

    void RegisterForCellUpdateBuffer();

    virtual void Initialize(IWFCManager* manager) override;

    virtual WFCCell* GetCell(WFCPosition* position) override;
    virtual std::unordered_set<WFCCell*> GetAlertees(const WFCPosition* positionOfInterest) override;
    virtual void RegisterForCellUpdates(WFCPosition* positionOfInterest, WFCCell* toRegister) override;
    virtual void DeRegisterForCellUpdates(WFCPosition* positionOfInterest, WFCCell* toDeregister) override;
    virtual void PrintGrid() override;
    virtual void Reset() override;
    virtual std::vector<unsigned long long>* IWFCGrid::Export() override;
};


