#include "WFCRuleCellIsNot.h"
#include "WFCCell.h"

bool WFCRuleCellIsNot::Test(WFCCellUpdate update, const WFCPosition& currentCellPosition)
{
    if (update.collapsedTo > 0) {
        WFCPosition localDirection = update.updatedCell - currentCellPosition;
        for (auto const& it : localPositionsWeCareAbout) {
            if (*it == localDirection) {
                return !((update.collapsedTo & goal) > 0);
            }
        }
    }
    return true;
}
