#include "stdafx.h"
#include "CategorizedLineMergeMachineBase.h"
#include "CleaveSequenceFactory.h"

void CategorizedLineMergeMachineBase::initialize(CleaveSequenceFactory* in_cleaveSequenceFactoryRef, CategorizedLineGroup* in_categorizedLineGroupRef)
{
	cleaveSequenceFactoryRef = in_cleaveSequenceFactoryRef;
	categorizedLineGroupRef = in_categorizedLineGroupRef;
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
			mergablePartialBoundsMap[mergablePartialBoundsIndex++] = lineToMove;
		}
		else if (categorizedLineGroupRecordVectorBegin->categorizedLineIntersectionType == IntersectionType::NON_BOUND)
		{
			CategorizedLine lineToMove = cleaveSequenceFactoryRef->fetchAndRemoveNonboundWithGroupMapLocationPush(
																		categorizedLineGroupRecordVectorBegin->categorizedLineIndex,
																		&locationVector);
			mergableNonboundMap[mergableNonboundIndex++] = lineToMove;
		}
		else if (categorizedLineGroupRecordVectorBegin->categorizedLineIntersectionType == IntersectionType::INTERCEPTS_POINT_PRECISE)
		{
			CategorizedLine lineToMove = cleaveSequenceFactoryRef->fetchAndRemoveInterceptPointPreciseWithGroupMapLocationPush(
																		categorizedLineGroupRecordVectorBegin->categorizedLineIndex,
																		&locationVector);
			mergableInterceptsPointPreciseMap[mergableInterceptsPointPreciseIndex++] = lineToMove;
		}
		else if (categorizedLineGroupRecordVectorBegin->categorizedLineIntersectionType == IntersectionType::A_SLICE)
		{
			//CategorizedLine lineToMove = cleaveSequenceFactoryRef->fetchAndRemoveASlice(categorizedLineGroupRecordVectorBegin->categorizedLineIndex);
			CategorizedLine lineToMove = cleaveSequenceFactoryRef->fetchAndRemoveASliceWithGroupMapLocationPush(
																		categorizedLineGroupRecordVectorBegin->categorizedLineIndex,
																		&locationVector);
			mergableASliceMap[mergableASliceIndex++] = lineToMove;
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