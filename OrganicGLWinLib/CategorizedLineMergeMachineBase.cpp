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
	auto categorizedLineGroupRecordVectorBegin = categorizedLineGroupRef->recordVector.begin();
	auto categorizedLineGroupRecordVectorEnd = categorizedLineGroupRef->recordVector.end();
	for (; categorizedLineGroupRecordVectorBegin != categorizedLineGroupRecordVectorEnd; categorizedLineGroupRecordVectorBegin++)
	{
		if (categorizedLineGroupRecordVectorBegin->categorizedLineIntersectionType == IntersectionType::PARTIAL_BOUND)
		{
			CategorizedLine lineToMove = cleaveSequenceFactoryRef->fetchAndRemovePartialBound(categorizedLineGroupRecordVectorBegin->categorizedLineIndex);
			mergablePartialBoundsMap[mergablePartialBoundsIndex++] = lineToMove;
		}
		else if (categorizedLineGroupRecordVectorBegin->categorizedLineIntersectionType == IntersectionType::NON_BOUND)
		{
			CategorizedLine lineToMove = cleaveSequenceFactoryRef->fetchAndRemoveNonbound(categorizedLineGroupRecordVectorBegin->categorizedLineIndex);
			mergableNonboundMap[mergableNonboundIndex++] = lineToMove;
		}
		else if (categorizedLineGroupRecordVectorBegin->categorizedLineIntersectionType == IntersectionType::INTERCEPTS_POINT_PRECISE)
		{
			CategorizedLine lineToMove = cleaveSequenceFactoryRef->fetchAndRemoveInterceptPointPrecise(categorizedLineGroupRecordVectorBegin->categorizedLineIndex);
			mergableInterceptsPointPreciseMap[mergableInterceptsPointPreciseIndex++] = lineToMove;
		}
		else if (categorizedLineGroupRecordVectorBegin->categorizedLineIntersectionType == IntersectionType::A_SLICE)
		{
			CategorizedLine lineToMove = cleaveSequenceFactoryRef->fetchAndRemoveASlice(categorizedLineGroupRecordVectorBegin->categorizedLineIndex);
			mergableASliceMap[mergableASliceIndex++] = lineToMove;
		}
	}
}

CategorizedLine CategorizedLineMergeMachineBase::fetchProducedLine()
{
	return mergedLineResult;
}