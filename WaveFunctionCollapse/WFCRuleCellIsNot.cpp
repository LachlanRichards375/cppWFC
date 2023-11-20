#include "WFCRuleCellIsNot.h"
#include "WFCCell.h"

WFCRuleCellIsNot::WFCRuleCellIsNot(unsigned long goal, std::vector<std::shared_ptr<WFCPosition>> localPositionsWeCareAbout)
    :IWFCRule(goal, localPositionsWeCareAbout)
{
}

bool WFCRuleCellIsNot::Test(WFCCellUpdate update, const WFCPosition& currentCellPosition)
{
    if (update.collapsedTo > 0) {
        WFCPosition localDirection = (*update.updatedCell) - (currentCellPosition);
        for (auto const& it : localPositionsWeCareAbout) {
            if (*it == localDirection) {
                return !((update.collapsedTo & goal) > 0);
            }
        }
    }
    return true;
}
