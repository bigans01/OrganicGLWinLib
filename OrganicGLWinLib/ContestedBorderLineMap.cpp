#include "stdafx.h"
#include "ContestableBorderLineMap.h"

void ContestableBorderLineMap::insertChallengingCategorizedLine(int in_borderLineID, CategorizedLine in_categorizedLine)
{
	contestableBorderLineChallengersMap[in_borderLineID].push_back(in_categorizedLine);
}