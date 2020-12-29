#include "stdafx.h"
#include "CategorizedLinePartialBoundMerger.h"
#include "CleaveSequenceFactory.h"

void CategorizedLinePartialBoundMerger::extractCategorizedLines()
{
	auto categorizedLineGroupRecordVectorBegin = categorizedLineGroupRef->recordVector.begin();
	auto categorizedLineGroupRecordVectorEnd = categorizedLineGroupRef->recordVector.end();
	for (; categorizedLineGroupRecordVectorBegin != categorizedLineGroupRecordVectorEnd; categorizedLineGroupRecordVectorBegin++)
	{
		if (categorizedLineGroupRecordVectorBegin->categorizedLineIntersectionType == IntersectionType::PARTIAL_BOUND)
		{
			//CategorizedLine lineToMove = cleaveSequenceFactoryRef->fetchAndRemovePartialBound(categorizedLineGroupRecordVectorBegin->categorizedLineIndex);
		}
		else if (categorizedLineGroupRecordVectorBegin->categorizedLineIntersectionType == IntersectionType::NON_BOUND)
		{
			//CategorizedLine lineToMove = cleaveSequenceFactoryRef->fetchAndRemoveNonbound(categorizedLineGroupRecordVectorBegin->categorizedLineIndex);
		}
		else if (categorizedLineGroupRecordVectorBegin->categorizedLineIntersectionType == IntersectionType::INTERCEPTS_POINT_PRECISE)
		{
			//CategorizedLine lineToMove = cleaveSequenceFactoryRef->fetchAndRemoveInterceptPointPrecise(categorizedLineGroupRecordVectorBegin->categorizedLineIndex);
		}
	}
}

void CategorizedLinePartialBoundMerger::runMerging()
{

}