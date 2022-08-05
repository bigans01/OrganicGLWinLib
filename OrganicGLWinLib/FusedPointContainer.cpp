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
		int nextIndexToInsert = int(fusedPointMap.size());
		fusedPointMap[nextIndexToInsert].point = in_point;
		fusedPointMap[nextIndexToInsert].insertFusedPointSubData(in_fusedPointSubData);
	}
}

void FusedPointContainer::rebuildSubDataForPointForTwoBorderLines(glm::vec3 in_point, std::map<int, int> in_lineMap)
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
		// remember, we don't want to change the origin; we're only going to be updating the border line flag and border line value,
		// and set the line index to 0.
		auto targetVectorBegin = fusedPointMap[foundIndex].subDataVector.begin();
		auto targetVectorEnd = fusedPointMap[foundIndex].subDataVector.end();
		auto currentBorderLineIDBegin = in_lineMap.begin();
		for (; targetVectorBegin != targetVectorEnd; targetVectorBegin++)
		{
			targetVectorBegin->isBorderLine = 1;
			targetVectorBegin->borderLineValue = currentBorderLineIDBegin->second;
			targetVectorBegin->triangleLineIndex = 0;
			currentBorderLineIDBegin++;
		}
	}
}

FusedPointMeta FusedPointContainer::retrieveFusedPointMeta(glm::vec3 in_pointToRetrieve, FusionCandidateOrigin in_originToMarkAs)
{
	FusedPointMeta returnMeta;
	auto pointFinderBegin = fusedPointMap.begin();
	auto pointFinderEnd = fusedPointMap.end();
	for (; pointFinderBegin != pointFinderEnd; pointFinderBegin++)
	{
		if (pointFinderBegin->second.point == in_pointToRetrieve)
		{
			returnMeta = pointFinderBegin->second.getMetaForPoint(in_pointToRetrieve);
		}
	}
	returnMeta.origin = in_originToMarkAs;
	returnMeta.point = in_pointToRetrieve;
	return returnMeta;
}

FusedPointMeta FusedPointContainer::retrieveOtherFusedPointMeta(glm::vec3 in_pointToNotRetrieve, FusionCandidateOrigin in_originToMarkAs)
{
	FusedPointMeta returnMeta;
	glm::vec3 fetchedPointToRetrieve;
	auto pointFinderBegin = fusedPointMap.begin();
	auto pointFinderEnd = fusedPointMap.end();
	for (; pointFinderBegin != pointFinderEnd; pointFinderBegin++)
	{
		if (pointFinderBegin->second.point != in_pointToNotRetrieve)	// remember, we're trying to find the other point that isn't equal to the one we passe din 
		{
			fetchedPointToRetrieve = pointFinderBegin->second.point;	// store the "other" point we found
			returnMeta = pointFinderBegin->second.getMetaForPoint(fetchedPointToRetrieve);
		}
	}
	returnMeta.origin = in_originToMarkAs;
	returnMeta.point = fetchedPointToRetrieve;
	return returnMeta;
}

std::vector<FusedPointSubData>* FusedPointContainer::fetchSubDataVectorForPoint(glm::vec3 in_pointToRetrieveFor)
{
	std::vector<FusedPointSubData>* returnRef = nullptr;
	auto pointFinderBegin = fusedPointMap.begin();
	auto pointFinderEnd = fusedPointMap.end();
	for (; pointFinderBegin != pointFinderEnd; pointFinderBegin++)
	{
		if (pointFinderBegin->second.point == in_pointToRetrieveFor)
		{
			returnRef = pointFinderBegin->second.getSubDataVectorRef();
		}
	}
	return returnRef;
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

void FusedPointContainer::loadPointsIntoOtherContainer(FusedPointContainer* in_otherFusedPointContainer)
{
	auto currentContainerBegin = fusedPointMap.begin();
	auto currentContainerEnd = fusedPointMap.end();
	for (; currentContainerBegin != currentContainerEnd; currentContainerBegin++)
	{
		glm::vec3 currentPoint = currentContainerBegin->second.point;
		//FusedPointSubData currentSubData = currentContainerBegin->second.
		auto subDataVectorBegin = currentContainerBegin->second.subDataVector.begin();
		auto subDataVectorEnd = currentContainerBegin->second.subDataVector.end();
		for (; subDataVectorBegin != subDataVectorEnd; subDataVectorBegin++)
		{
			in_otherFusedPointContainer->insertSubDataForPoint(currentPoint, *subDataVectorBegin);
		}
	}
}