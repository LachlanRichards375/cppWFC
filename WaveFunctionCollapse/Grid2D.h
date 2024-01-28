#pragma once
#include "IWFCGrid.h"
#include <memory>
#include <unordered_set>
#include "ThreadSafeQueue.h"
#include <map>

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
    //2d vector of a list of shared pointers
    std::vector<std::vector<std::unordered_set<WFCCell*>>> cellsToUpdate;

    //Threaded
    mutable std::mutex job_count_mutex;
    std::condition_variable c;
    bool initializing;
    SafeQueue<WFCCell*> cellsToSetup;
    std::map<std::thread::id, int> bufferIndexMap;
    std::vector<std::vector<BufferNotification>> buffers;
    volatile int ruleSetupsFinished;
    void RuleSetupJob(int bufferIndex);
    void WaitForRulesToSetup();
public:
    Grid2D(WFCPosition& newSize);
    virtual void Initialize(IWFCManager* manager) override;

    virtual WFCCell* GetCell(WFCPosition* position) override;
    virtual std::unordered_set<WFCCell*> GetAlertees(const WFCPosition* positionOfInterest) override;
    virtual void RegisterForCellUpdates(WFCPosition* positionOfInterest, WFCCell* toRegister) override;
    virtual void DeRegisterForCellUpdates(WFCPosition* positionOfInterest, WFCCell* toDeregister) override;
    virtual void PrintGrid() override;
    virtual void Reset() override;
    virtual std::vector<unsigned long long>* IWFCGrid::Export() override;
};


