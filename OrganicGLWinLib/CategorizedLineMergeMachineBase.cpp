#include "stdafx.h"
#include "CategorizedLineMergeMachineBase.h"
#include "CleaveSequenceFactory.h"

void CategorizedLineMergeMachineBase::initialize(CleaveSequenceFactory* in_cleaveSequenceFactoryRef, CategorizedLineGroup* in_categorizedLineGroupRef, PolyDebugLevel in_polyDebugLevel)
{
	cleaveSequenceFactoryRef = in_cleaveSequenceFactoryRef;
	categorizedLineGroupRef = in_categorizedLineGroupRef;
	mergeMachineLogger.setDebugLevel(in_polyDebugLevel);
}

void CategorizedLineMergeMachineBase::extractCategorizedLines()
{
	std::vector<CategorizedLineGroupLocation> locationVector;
	auto categorizedLineGroupRecordVectorBegin = categorizedLineGroupRef->recordVector.begin();
	auto categorizedLineGroupRecordVectorEnd = categorizedLineGroupRef->recordVector.end();
	for (; categorizedLineGroupRecordVectorBegin != categorizedLineGroupRecordVectorEnd; categorizedLineGroupRecordVectorBegin++)
	{
		if (categorizedLineGroupRecordVectorBegin->categorizedLineIntersectionType == IntersectionType::PARTIAL_BOUND)
		{
			CategorizedLine lineToMove = cleaveSequenceFactoryRef->fetchAndRemovePartialBoundWithGroupMapLocationPush(
																		categorizedLineGroupRecordVectorBegin->categorizedLineIndex,
																		&locationVector);
			mergablePartialBoundsMap[mergablePartialBoundsCount++] = lineToMove;
		}
		else if (categorizedLineGroupRecordVectorBegin->categorizedLineIntersectionType == IntersectionType::NON_BOUND)
		{
			CategorizedLine lineToMove = cleaveSequenceFactoryRef->fetchAndRemoveNonboundWithGroupMapLocationPush(
																		categorizedLineGroupRecordVectorBegin->categorizedLineIndex,
																		&locationVector);
			mergableNonboundMap[mergableNonboundCount++] = lineToMove;
		}
		else if (categorizedLineGroupRecordVectorBegin->categorizedLineIntersectionType == IntersectionType::INTERCEPTS_POINT_PRECISE)
		{
			CategorizedLine lineToMove = cleaveSequenceFactoryRef->fetchAndRemoveInterceptPointPreciseWithGroupMapLocationPush(
																		categorizedLineGroupRecordVectorBegin->categorizedLineIndex,
																		&locationVector);
			mergableInterceptsPointPreciseMap[mergableInterceptsPointPreciseCount++] = lineToMove;
		}
		else if (categorizedLineGroupRecordVectorBegin->categorizedLineIntersectionType == IntersectionType::A_SLICE)
		{
			CategorizedLine lineToMove = cleaveSequenceFactoryRef->fetchAndRemoveASliceWithGroupMapLocationPush(
																		categorizedLineGroupRecordVectorBegin->categorizedLineIndex,
																		&locationVector);
			mergableASliceMap[mergableASliceCount++] = lineToMove;
		}
		else if (categorizedLineGroupRecordVectorBegin->categorizedLineIntersectionType == IntersectionType::A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE)
		{
			std::cout << ">>>>>>>>>>> Found A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE. " << std::endl;
			cleaveSequenceFactoryRef->printLineCounts();
			CategorizedLine lineToMove = cleaveSequenceFactoryRef->fetchAndRemoveASliceSingleInterceptsPointPreciseWithGroupMapLocationPush(
																		categorizedLineGroupRecordVectorBegin->categorizedLineIndex,
																		&locationVector);
			mergableASliceSingleInterceptsPointPreciseMap[mergableASliceSingleInterceptsPointPreciseCount++] = lineToMove;
		}
	}

	// remove the records from the group, when we're done extracting the categorized lines.
	auto locationVectorBegin = locationVector.begin();
	auto locationVectorEnd = locationVector.end();
	for (; locationVectorBegin != locationVectorEnd; locationVectorBegin++)
	{
		categorizedLineGroupRef->removeRecord(locationVectorBegin->locationIntersectionType, locationVectorBegin->locationPoolIndex);
	}
}

CategorizedLine CategorizedLineMergeMachineBase::fetchProducedLine()
{
	return mergedLineResult;
}

CategorizedLine CategorizedLineMergeMachineBase::fetchAndRemovePartialBoundMergeCandidate(int in_fetchIndex)
{
	CategorizedLine returnLine = mergablePartialBoundsMap[in_fetchIndex];
	mergablePartialBoundsMap.erase(in_fetchIndex);
	mergablePartialBoundsCount--;
	return returnLine;
}

CategorizedLine CategorizedLineMergeMachineBase::fetchAndRemoveASliceMergeCandidate(int in_fetchIndex)
{
	CategorizedLine returnLine = mergableASliceMap[in_fetchIndex];
	mergableASliceMap.erase(in_fetchIndex);
	mergableASliceCount--;
	return returnLine;
}

CategorizedLine CategorizedLineMergeMachineBase::fetchAndRemoveASliceSingleInterceptsPointPreciseCandidate(int in_fetchIndex)
{
	CategorizedLine returnLine = mergableASliceSingleInterceptsPointPreciseMap[in_fetchIndex];
	mergableASliceSingleInterceptsPointPreciseMap.erase(in_fetchIndex);
	mergableASliceSingleInterceptsPointPreciseCount--;
	return returnLine;
}

CategorizedLine CategorizedLineMergeMachineBase::fetchAndRemoveNonboundMergeCandidate(int in_fetchIndex)
{
	CategorizedLine returnLine = mergableNonboundMap[in_fetchIndex];
	mergableNonboundMap.erase(in_fetchIndex);
	mergableNonboundCount--;
	return returnLine;
}

CategorizedLine CategorizedLineMergeMachineBase::fetchAndRemoveInterceptsPointPreciseMergeCandidate(int in_fetchIndex)
{
	CategorizedLine returnLine = mergableInterceptsPointPreciseMap[in_fetchIndex];
	mergableInterceptsPointPreciseMap.erase(in_fetchIndex);
	mergableInterceptsPointPreciseCount--;
	return returnLine;
}

CategorizedLineSearchResult CategorizedLineMergeMachineBase::checkForNextNonboundMergeCandidate(glm::vec3 in_pointToSearch)
{
	CategorizedLineSearchResult searchResult;
	if (mergableNonboundCount > 0)		// search for categorized lines, but only if there are ones to search for.
	{
		// search through all the non bound lines.
		auto nonboundBegin = mergableNonboundMap.begin();
		auto nonboundEnd = mergableNonboundMap.end();
		bool wasFound = false;	// was a match found? 
		int foundIndex = 0;		// if it was found, what was its index?
		IRPointType pointCheckResult = IRPointType::NEITHER;	// starts out as NEITHER
		for (nonboundBegin; nonboundBegin != nonboundEnd; nonboundBegin++)
		{
			pointCheckResult = nonboundBegin->second.checkIfPointIsInLine(in_pointToSearch);
			if (pointCheckResult != IRPointType::NEITHER) // it was found (it's either A or B)
			{
				wasFound = true;					// set the found flag.
				foundIndex = nonboundBegin->first;	// store the index.
				break;								// exit for loop when found.
			}

			//std::cout << "One iteration passed through nonbounds..." << std::endl;
		}

		// if it was found, do this stuff (should be able to ignore invalid iterators this way)
		if (wasFound == true)
		{
			searchResult.wasFound = true;			// set the search result found value to be true
			CategorizedLine foundLine = fetchAndRemoveNonboundMergeCandidate(foundIndex);		// remove from the nonbound.
			if (pointCheckResult == IRPointType::POINT_B)
			{
				foundLine.line.swapToA();	// it's backwards; so swap the lines.

			}
			searchResult.returnLine = foundLine;	// store the appropriate line
			searchResult.nextPointToFind = foundLine.line.pointB;	// set the next point to find.


			//std::cout << "!! Point (next line) was found! " << std::endl;
			//std::cout << "Point A: " << foundLine.line.pointA.x << ", " << foundLine.line.pointA.y << ", " << foundLine.line.pointA.z << " | Point B: " << foundLine.line.pointB.x << ", " << foundLine.line.pointB.y << ", " << foundLine.line.pointB.z << std::endl;
			//std::cout << "Number of remaining nonbounds: " << nonboundCount << std::endl;
		}

	}
	else
	{
		searchResult.wasFound = false;
	}

	return searchResult;
}

CategorizedLineSearchResult CategorizedLineMergeMachineBase::getClosestNonBoundMergeCandidate(glm::vec3 in_pointToSearch)
{
	CategorizedLineSearchResult searchResult;

	int currentClosestID = 0;			// stores the index of the closest of the nonbound
	float currentDistance = 10000.0f;	// the current distance.
	glm::vec3 closestPointValue;
	if (mergableNonboundCount > 0)		// search for categorized lines, but only if there are ones to search for.
	{
		auto findClosestBegin = mergableNonboundMap.begin();
		auto findClosestEnd = mergableNonboundMap.end();
		for (; findClosestBegin != findClosestEnd; findClosestBegin++)
		{
			//if (glm::distance(in_pointToCompareAgainst, recordMapBegin->second) < distance)
			//{
				//shortestID = recordMapBegin->first;
				//distance = glm::distance(in_pointToCompareAgainst, recordMapBegin->second);
			//};
			if (glm::distance(in_pointToSearch, findClosestBegin->second.line.pointA) < currentDistance)
			{
				currentClosestID = findClosestBegin->first;
				currentDistance = glm::distance(in_pointToSearch, findClosestBegin->second.line.pointA);
				closestPointValue = findClosestBegin->second.line.pointA;
			}
			if (glm::distance(in_pointToSearch, findClosestBegin->second.line.pointB) < currentDistance)
			{
				currentClosestID = findClosestBegin->first;
				currentDistance = glm::distance(in_pointToSearch, findClosestBegin->second.line.pointB);
				closestPointValue = findClosestBegin->second.line.pointB;
			}
		}

		// once we've acquired the closest point, search on that
		// search through all the non bound lines.
		auto nonboundBegin = mergableNonboundMap.begin();
		auto nonboundEnd = mergableNonboundMap.end();
		bool wasFound = false;	// was a match found? 
		int foundIndex = 0;		// if it was found, what was its index?
		IRPointType pointCheckResult = IRPointType::NEITHER;	// starts out as NEITHER
		for (nonboundBegin; nonboundBegin != nonboundEnd; nonboundBegin++)
		{
			pointCheckResult = nonboundBegin->second.checkIfPointIsInLine(closestPointValue);
			if (pointCheckResult != IRPointType::NEITHER) // it was found (it's either A or B)
			{
				wasFound = true;					// set the found flag.
				foundIndex = nonboundBegin->first;	// store the index.
				break;								// exit for loop when found.
			}

			//std::cout << "One iteration passed through nonbounds..." << std::endl;
		}

		// if it was found, do this stuff (should be able to ignore invalid iterators this way)
		if (wasFound == true)
		{
			searchResult.wasFound = true;			// set the search result found value to be true
			CategorizedLine foundLine = fetchAndRemoveNonboundMergeCandidate(foundIndex);		// remove from the nonbound.
			if (pointCheckResult == IRPointType::POINT_B)
			{
				foundLine.line.swapToA();	// it's backwards; so swap the lines.

			}
			searchResult.returnLine = foundLine;	// store the appropriate line
			searchResult.nextPointToFind = foundLine.line.pointB;	// set the next point to find.


			//std::cout << "!! Point (next line) was found! " << std::endl;
			//std::cout << "Point A: " << foundLine.line.pointA.x << ", " << foundLine.line.pointA.y << ", " << foundLine.line.pointA.z << " | Point B: " << foundLine.line.pointB.x << ", " << foundLine.line.pointB.y << ", " << foundLine.line.pointB.z << std::endl;
			//std::cout << "Number of remaining nonbounds: " << nonboundCount << std::endl;
		}

	}
	else
	{
		std::cout << "(CategorizedLineMergeMachineBase): WARNING, attempted to get closets nonbound merge candidate, but no candidates found; program will now HALT in infinite loop." << std::endl;
		int someVal = 3;
		while (someVal == 3)
		{

		}
	}
	return searchResult;
}