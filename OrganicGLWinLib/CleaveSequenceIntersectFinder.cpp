#include "stdafx.h"
#include "CleaveSequenceIntersectFinder.h"

CleaveSequenceIntersectFinder::CleaveSequenceIntersectFinder(SPoly* in_sPolyRef)
{
	sPolyRef = in_sPolyRef;
	loadInterceptRecords();

	int cleaveMapSize = in_sPolyRef->cleaveMap.size();
	auto cleaveBegin = in_sPolyRef->cleaveMap.begin();
	auto cleaveEnd = in_sPolyRef->cleaveMap.end();

	std::cout << "~~~ cleave map size: " << cleaveMapSize << std::endl;

	for (cleaveBegin; cleaveBegin != cleaveEnd; cleaveBegin++)
	{
		/*
		auto sequenceBegin = cleaveBegin->second.cleavingLines.begin();
		auto sequenceEnd = cleaveBegin->second.cleavingLines.end();
		for (sequenceBegin; sequenceBegin != sequenceEnd; sequenceBegin++)
		{
			std::cout << sequenceBegin->first << ": isOnBorderLine " << sequenceBegin->second.line.numberOfBorderLines 
				<< " | isPointAOnBorder: " << sequenceBegin->second.line.isPointAOnBorder << " | pointABorder: " << sequenceBegin->second.line.pointABorder 
				<< " || isPointBOnBorder: " << sequenceBegin->second.line.isPointBOnBorder << " | pointBBorder: " << sequenceBegin->second.line.pointBBorder << std::endl;

			
			// check to see if the current categorized line has a value of two (or three?) for its numberOfBorderLines; if it's 1, check the cycling.
			// *** below lines are test only, will not be used here ***
			if (sequenceBegin->second.line.numberOfBorderLines == 1)
			{
				BorderLineCycler lineCycler(in_sPolyRef);
				int borderLineId = sequenceBegin->second.line.getBorderLineIDFromSingularBorderLineCount();
				lineCycler.findCyclingDirection(&in_sPolyRef->borderLines[borderLineId], borderLineId, &sequenceBegin->second);	// find the direction to go off of, based off the border line
				lineCycler.buildCycle(in_sPolyRef, borderLineId);		// build the cycle, once the direction is found.
			}
		}
		*/

		auto sequenceBegin = cleaveBegin->second.cleavingLines.begin();	// get first element
		auto sequenceEnd = cleaveBegin->second.cleavingLines.rbegin();	// get last element

		// if the first line in the sequence is only one border line, do this; test
		if (sequenceBegin->second.line.numberOfBorderLines == 1)
		{
			BorderLineCycler lineCycler(in_sPolyRef);
			int beginLineborderLineId = sequenceBegin->second.line.getBorderLineIDFromSingularBorderLineCount();
			int endLineborderLineId = sequenceEnd->second.line.getBorderLineIDFromSingularBorderLineCount();
			lineCycler.findCyclingDirection(&in_sPolyRef->borderLines[beginLineborderLineId], beginLineborderLineId, &sequenceBegin->second);	// find the direction to go off of, based off the border line
			lineCycler.buildCycle(in_sPolyRef, beginLineborderLineId, endLineborderLineId);		// build the cycle, once the direction is found.
		}

	}
}

void CleaveSequenceIntersectFinder::loadInterceptRecords()
{
	auto cleaveBegin = sPolyRef->cleaveMap.begin();
	auto cleaveEnd = sPolyRef->cleaveMap.end();
	for (cleaveBegin; cleaveBegin != cleaveEnd; cleaveBegin++)		// cycle through each CleaveSequence
	{
		auto sequenceBegin = cleaveBegin->second.cleavingLines.begin();
		auto sequenceEnd = cleaveBegin->second.cleavingLines.end();
		for (sequenceBegin; sequenceBegin != sequenceEnd; sequenceBegin++)	// cycle through each CategorizedLine in each CleaveSequence
		{
			if (sequenceBegin->second.line.numberOfBorderLines == 1)		// if it contains one interception, load the data into appropriate border line's BorderLineIntersectRecorder
			{
				int borderLineId = sequenceBegin->second.line.getBorderLineIDFromSingularBorderLineCount();		// get the appropriate border line to insert data into.
				SPolyBorderLines* borderLineRef = &sPolyRef->borderLines[borderLineId];								// get the line ref.
				borderLineRef->intersectRecorder.insertNewRecord(cleaveBegin->first, sequenceBegin->first, &sequenceBegin->second);		// first argument: the ID of the cleave sequence
																																		// second argument: the ID of the categorized line we're inserting from the sequence
																																		// third argument: a reference to the categorized line itself
				std::cout << "## BorderLine updated, BorderLineID: " << borderLineId << " SequenceID: " << cleaveBegin->first << " | CategorizedLineID: " << sequenceBegin->first << std::endl;
			}	
		}
	}
}