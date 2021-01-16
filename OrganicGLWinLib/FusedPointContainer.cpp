#include "stdafx.h"
#include "FusedPointContainer.h"

void FusedPointContainer::insertSubDataForPoint(glm::vec3 in_point, FusedPointSubData in_fusedPointSubData)
{
	// check if the point exists
	bool wasFound = false;
	int foundIndex = 0;
	auto pointFinderBegin = fusedPointMap.begin();
	auto pointFinderEnd = fusedPointMap.end();
	for (; pointFinderBegin != pointFinderEnd; pointFinderBegin++)
	{
		if (pointFinderBegin->second.point == in_point)
		{
			wasFound = true;
			foundIndex = pointFinderBegin->first;
		}
	}

	// if it does exist, just add to the one at the index we found it at.
	if (wasFound == true)
	{
		fusedPointMap[foundIndex].insertFusedPointSubData(in_fusedPointSubData);
	}
	else if (wasFound == false)
	{
		int nextIndexToInsert = fusedPointMap.size();
		fusedPointMap[nextIndexToInsert].point = in_point;
		fusedPointMap[nextIndexToInsert].insertFusedPointSubData(in_fusedPointSubData);
	}
}

void FusedPointContainer::clearFusedPoints()
{
	fusedPointMap.clear();
}

void FusedPointContainer::printFusedPoints()
{
	auto fusedPrintBegin = fusedPointMap.begin();
	auto fusedPrintEnd = fusedPointMap.end();
	for (; fusedPrintBegin != fusedPrintEnd; fusedPrintBegin++)
	{
		fusedPrintBegin->second.printData();
	}
}