#include "stdafx.h"
#include "CleaveSequenceMeta.h"
#include "CleaveSequence.h"

int CleaveSequenceMeta::acquireNextBorderLineID()
{
	return cleaveSequencePtr->retrievedLineEndpointBorderLine(crawlDirection);
}

void CleaveSequenceMeta::determineCrawlDirectionFromPoint(glm::vec3 in_pointToDetermineFrom)
{
	if (sequenceType == CleaveSequenceType::SINGLE_LINE)
	{
		if (in_pointToDetermineFrom == cleaveSequencePtr->cleavingLines.begin()->second.line.pointA)		// if the point to determine equals point A, we go FORWARD.
		{
			crawlDirection = CleaveSequenceCrawlDirection::FORWARD;
		}
		else if (in_pointToDetermineFrom == cleaveSequencePtr->cleavingLines.begin()->second.line.pointB)	// otherwise, it must be point B in this singular line, so go reverse.
		{
			crawlDirection = CleaveSequenceCrawlDirection::REVERSE;
		}
	}
	else if (sequenceType == CleaveSequenceType::MULTI_LINE)
	{
		if (in_pointToDetermineFrom == cleaveSequencePtr->cleavingLines.begin()->second.line.pointA)		// if the point to determine equals point A, we go FORWARD.
		{
			std::cout << "|||> Crawl direction is FORWARD!" << std::endl;
			crawlDirection = CleaveSequenceCrawlDirection::FORWARD;
		}
		else if (in_pointToDetermineFrom == cleaveSequencePtr->cleavingLines.rbegin()->second.line.pointB)	// otherwise, it must be point B in the line at the end of the CategorizedLines (cleavingLines)
		{
			std::cout << "|||> Crawl direction is BACKWARD!" << std::endl;
			crawlDirection = CleaveSequenceCrawlDirection::REVERSE;
		}
	}
}

CategorizedLine CleaveSequenceMeta::fetchNextCategorizedLineInSequence()
{
	CategorizedLine returnLine;
	if (crawlDirection == CleaveSequenceCrawlDirection::FORWARD)
	{
		auto categorizedLinesBegin = cleaveSequencePtr->cleavingLines.begin();	// start from the BEGINNING of the map.
		for (int x = 0; x < currentLineIndex; x++)		// iterate an amount of times equal to the current line index, to get to the next line
														// revisit this later; is there an easier way to do this?
		{
			categorizedLinesBegin++;
		}
		returnLine = categorizedLinesBegin->second;
	}
	else if (crawlDirection == CleaveSequenceCrawlDirection::REVERSE)
	{
		std::cout << "### Entered REVERSE crawl else/if... " << std::endl;
		std::cout << "Number of lines to crawl: " << cleaveSequencePtr->cleavingLines.size() << std::endl;
		std::cout << "Current line index: " << currentLineIndex << std::endl;
		auto categorizedLinesBegin = cleaveSequencePtr->cleavingLines.rbegin();	// start from the END of the map.
		for (int x = 0; x < currentLineIndex; x++)		// iterate an amount of times equal to the current line index, to get to the next line
														// revisit this later; is there an easier way to do this?
		{
			std::cout << "crawl iteration..." << std::endl;
			categorizedLinesBegin++;
		}
		returnLine = categorizedLinesBegin->second;

		// swap to A, since we're going into a reverse crawl direction. 
		returnLine.line.swapToA();

	}
	currentLineIndex++;
	numberOfRemainingLines--;
	return returnLine;
}

CategorizedLine CleaveSequenceMeta::fetchFirstCategorizedLineForWelder()
{
	auto categorizedLinesBegin = cleaveSequencePtr->cleavingLines.rbegin();	// get the last line in the sequence.
	CategorizedLine returnLine = categorizedLinesBegin->second;
	numberOfRemainingLines--;
	return returnLine;
}

bool CleaveSequenceMeta::checkIfCleaveSequenceHasRunallCategorizedLines()
{
	return (currentLineIndex == numberOfLines);
}