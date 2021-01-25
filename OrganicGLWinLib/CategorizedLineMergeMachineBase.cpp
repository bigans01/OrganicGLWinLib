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
			//CategorizedLine lineToMove = cleaveSequenceFactoryRef->fetchAndRemovePartialBound(categorizedLineGroupRecordVectorBegin->categorizedLineIndex);
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
			//CategorizedLine lineToMove = cleaveSequenceFactoryRef->fetchAndRemoveASlice(categorizedLineGroupRecordVectorBegin->categorizedLineIndex);
			CategorizedLine lineToMove = cleaveSequenceFactoryRef->fetchAndRemoveASliceWithGroupMapLocationPush(
																		categorizedLineGroupRecordVectorBegin->categorizedLineIndex,
																		&locationVector);
			mergableASliceMap[mergableASliceCount++] = lineToMove;
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