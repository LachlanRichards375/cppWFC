#include "IWFCRule.h"

IWFCRule::IWFCRule(unsigned long goal, std::vector< std::shared_ptr<WFCPosition>> localPositionsWeCareAbout)
{
	IWFCRule::goal = goal;
	IWFCRule::localPositionsWeCareAbout = localPositionsWeCareAbout;
}

unsigned long const IWFCRule::GetGoal()
{
	return goal;
}

std::vector< std::shared_ptr<WFCPosition>> IWFCRule::GetPositions()
{
	return localPositionsWeCareAbout;
}
