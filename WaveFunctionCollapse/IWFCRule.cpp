#include "IWFCRule.h"

IWFCRule::IWFCRule(unsigned long long goal, unsigned long long tile, std::vector< WFCPosition*> localPositionsWeCareAbout)
{
	IWFCRule::goal = goal;
	IWFCRule::tile = tile;
	IWFCRule::localPositionsWeCareAbout = localPositionsWeCareAbout;
}

unsigned long long const IWFCRule::GetGoal()
{
	return goal;
}

unsigned long long const IWFCRule::GetTile()
{
	return tile;
}

std::vector< WFCPosition*> IWFCRule::GetPositions()
{
	return localPositionsWeCareAbout;
}
