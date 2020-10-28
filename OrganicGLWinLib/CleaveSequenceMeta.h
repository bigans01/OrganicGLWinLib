#pragma once

#ifndef CLEAVESEQUENCEMETA_H
#define CLEAVESEQUENCEMETA_H

#include "CleaveSequenceType.h"
#include "CategorizedLine.h"
#include "CleaveSequenceCrawlDirection.h"

class CleaveSequence;		// for class forwarding
class CleaveSequenceMeta
{
	public:
		CleaveSequenceMeta() {};
		CleaveSequenceMeta(CleaveSequenceType  in_cleaveSequenceType, int in_numberOfLines, CleaveSequence* in_cleaveSequencePtr) :
			sequenceType(in_cleaveSequenceType),
			numberOfLines(in_numberOfLines),
			numberOfRemainingLines(in_numberOfLines),
			cleaveSequencePtr(in_cleaveSequencePtr)
		{};
		CleaveSequenceType sequenceType = CleaveSequenceType::UNDEFINED;	// undefined by default, set by initialization
		CleaveSequence* cleaveSequencePtr = nullptr;						// set by initialization
		int numberOfLines = 0;												// set by initialization; the number of lines in the sequence.
		int numberOfRemainingLines = 0;										// set by initialization; the number of fetchable lines remaining.
		int currentLineIndex = 0;												
		CleaveSequenceCrawlDirection crawlDirection = CleaveSequenceCrawlDirection::NOVAL;	// nothing is set by default, but should always become FORWARD or REVERSE
																							// before the crawling begins.
		int acquireNextBorderLineID();		// fetched when we're on the last line in the CleaveSequence
		void determineCrawlDirectionFromPoint(glm::vec3 in_pointToDetermineFrom);
		CategorizedLine fetchNextCategorizedLineInSequence();
		CategorizedLine fetchFirstCategorizedLineForWelder();
		bool checkIfCleaveSequenceHasRunallCategorizedLines();
};

#endif
