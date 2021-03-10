#include "stdafx.h"
#include "NonBoundMerger.h"
#include "CleaveSequenceFactory.h"

void NonBoundMerger::runMerging()
{
	mergeMachineLogger.log("(NonBoundMerger) Running merge fo MERGE_TO_NON_BOUND (1 line found). ", "\n");

	// load all points from all nonbounds into the counter map.
	auto nonboundsBegin = mergableNonboundMap.begin();
	auto nonboundsEnd = mergableNonboundMap.end();
	for (; nonboundsBegin != nonboundsEnd; nonboundsBegin++)
	{
		insertPointIntoCounterMap(nonboundsBegin->second.line.pointA);
		insertPointIntoCounterMap(nonboundsBegin->second.line.pointB);
	}

	// now, find the first point that is only registered once.
	glm::vec3 beginningPointToUse = findFirstPointWithSingularCount();

	// find the non-bound line which has this point.
	auto beginPointSearchBegin = mergableNonboundMap.begin();
	auto beginPointSearchEnd = mergableNonboundMap.end();
	int selectedCategorizedLineIndex = 0;
	bool swapNeeded = false;		// do this, in the event that the line we find matches it's point B to the beginningPointToUse.
	for (; beginPointSearchBegin != beginPointSearchEnd; beginPointSearchBegin++)
	{
		if
		(
			(beginPointSearchBegin->second.line.pointA == beginningPointToUse)
		)
		{
			selectedCategorizedLineIndex = beginPointSearchBegin->first;	// get the non-bound's ID.
			break;
		}
		else if
		(
			(beginPointSearchBegin->second.line.pointB == beginningPointToUse)
		)
		{
			swapNeeded = true;		// we must swap the found point B to A, after we acquire the line.
			selectedCategorizedLineIndex = beginPointSearchBegin->first;	// ""
			break;
		}
	}



	// next, extract the non-bound line we found; be sure to swap it if need be.
	CategorizedLine firstNonBoundLine = fetchAndRemoveNonboundMergeCandidate(selectedCategorizedLineIndex);
	if (swapNeeded == true)
	{
		firstNonBoundLine.line.swapToA();	// swap the line, if needed.
	}

	// insert the first line.
	insertLineIntoOrderedNonBounds(firstNonBoundLine);

	// acquire point B from the line; this is the first point to search for.
	glm::vec3 pointToSearch = orderedNonboundMap[0].line.pointB;
	while (mergableNonboundCount > 0)
	{
		//std::cout << "!! Searching for point: " << pointToSearch.x << ", " << pointToSearch.y << ", " << pointToSearch.z << std::endl;
		mergeMachineLogger.log("(NonBoundMerger) !! Searching for point: ", pointToSearch.x, ", ", pointToSearch.y, ", ", pointToSearch.z, "\n");
		CategorizedLineSearchResult result = checkForNextNonboundMergeCandidate(pointToSearch);
		if (result.wasFound == true)
		{
			//std::cout << "!! Point A of the found non-bound is: " << result.returnLine.line.pointA.x << ", " << result.returnLine.line.pointA.y << ", " << result.returnLine.line.pointA.z << std::endl;
			//std::cout << "!! Point B of the found non-bound is: " << result.returnLine.line.pointB.x << ", " << result.returnLine.line.pointB.y << ", " << result.returnLine.line.pointB.z << std::endl;
			mergeMachineLogger.log("(NonBoundMerger) !! Point A of the found non-bound is: ", result.returnLine.line.pointA.x, ", ", result.returnLine.line.pointA.y, ", ", result.returnLine.line.pointA.z, "\n");
			mergeMachineLogger.log("(NonBoundMerger) !! Point B of the found non-bound is: ", result.returnLine.line.pointB.x, ", ", result.returnLine.line.pointB.y, ", ", result.returnLine.line.pointB.z, "\n");
			pointToSearch = result.returnLine.line.pointB;
		}
	}

	// set up the mergedLineResult.
	mergedLineResult.type = IntersectionType::NON_BOUND;

	mergedLineResult.line.pointA = orderedNonboundMap[0].line.pointA;
	mergedLineResult.line.pointB = pointToSearch;
	mergedLineResult.parentPoly = orderedNonboundMap[0].parentPoly;
	mergedLineResult.emptyNormal = orderedNonboundMap[0].emptyNormal;

}

void NonBoundMerger::insertPointIntoCounterMap(glm::vec3 in_point)
{
	bool wasPointAlreadyFound = false;
	auto pointCountersBegin = pointCounterMap.begin();
	auto pointCountersEnd = pointCounterMap.end();
	for (; pointCountersBegin != pointCountersEnd; pointCountersBegin++)
	{
		if (pointCountersBegin->second.point == in_point)
		{
			pointCountersBegin->second.incrementPointCount();
			wasPointAlreadyFound = true;
		}
	}
	
	// insert the point if it didn't exist already.
	if (wasPointAlreadyFound == false)
	{
		int pointCounterSize = pointCounterMap.size();
		NonBoundMerger::PointCounter newCounter(in_point);
		pointCounterMap[pointCounterSize] = newCounter;
		pointCounterMap[pointCounterSize].incrementPointCount();
	}
}

glm::vec3 NonBoundMerger::findFirstPointWithSingularCount()
{
	glm::vec3 returnVec;
	auto singularPointFinderBegin = pointCounterMap.begin();
	auto singularPointFinderEnd = pointCounterMap.end();
	for (; singularPointFinderBegin != singularPointFinderEnd; singularPointFinderBegin++)
	{
		if (singularPointFinderBegin->second.pointCount == 1)
		{
			returnVec = singularPointFinderBegin->second.point;
			break;
		}
	}
	return returnVec;
}

void NonBoundMerger::insertLineIntoOrderedNonBounds(CategorizedLine in_categorizedLine)
{
	int currentOrderedIndex = orderedNonboundMap.size();
	orderedNonboundMap[currentOrderedIndex] = in_categorizedLine;
}