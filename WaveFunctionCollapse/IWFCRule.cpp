#include "IWFCRule.h"

IWFCRule::IWFCRule(unsigned long goal, std::vector<WFCPosition*> localPositionsWeCareAbout)
{
	IWFCRule::goal = goal;
	IWFCRule::localPositionsWeCareAbout = localPositionsWeCareAbout;
}

unsigned long const IWFCRule::GetGoal()
{
	return goal;
}
