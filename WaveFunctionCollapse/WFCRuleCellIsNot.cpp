#include "WFCRuleCellIsNot.h"
#include "WFCCell.h"

WFCRuleCellIsNot::WFCRuleCellIsNot(unsigned long long goal, unsigned long long tile, std::vector<WFCPosition*> localPositionsWeCareAbout)
    :IWFCRule(goal, tile, localPositionsWeCareAbout)
{
}

bool WFCRuleCellIsNot::Test(WFCCellUpdate update, const WFCPosition* currentCellPosition)
{
    if (update.collapsedTo > 0 && (update.collapsedTo & goal) > 0) {
        WFCPosition localDirection = (*update.updatedCell) - (currentCellPosition);
        for (auto const& it : localPositionsWeCareAbout) {
            if (*it == localDirection) {
                return (update.collapsedTo & goal) == 0;
            }
        }
    }
    return true;
}

std::string WFCRuleCellIsNot::to_string()
{
    std::string returner{ std::to_string(tile) + " Is Not " + std::to_string(goal) };
    for (auto& localPos : localPositionsWeCareAbout) {
        returner += ", " + localPos->to_string();
    }
    return returner + "\n";
}
