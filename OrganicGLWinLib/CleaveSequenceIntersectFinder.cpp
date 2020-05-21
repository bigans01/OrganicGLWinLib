#include "stdafx.h"
#include "CleaveSequenceIntersectFinder.h"

CleaveSequenceIntersectFinder::CleaveSequenceIntersectFinder(SPoly* in_sPolyRef)
{
	sPolyRef = in_sPolyRef;
	int cleaveMapSize = in_sPolyRef->cleaveMap.size();
	

	// only perform the check if the size is >= 1; we don't need to do any work if there aren't any cleave sequences.
	if (cleaveMapSize >= 1)
	{
		std::cout << "#- Records              > CleaveSequences found in cleaveMap; inserting intercept records into appropriate border lines...  " << cleaveMapSize << std::endl;
		loadInterceptRecords();		// load the data about the CleaveSequence's categorized lines, and the border lines they intercept, into the appropriate border line's BorderLineInterceptRecorders.
		auto cleaveBegin = in_sPolyRef->cleaveMap.begin();

		// strange, false positive error with cleavingLines below (5/16/2020)
		auto sequenceBegin = cleaveBegin->second.cleavingLines.begin();	// get first element  (the border line in the cycle where the cycle begins, fetched from the first CategorizedLine in the sequence)
		auto sequenceEnd = cleaveBegin->second.cleavingLines.rbegin();	// get last element  (the border line in the cycle where the cycle ends, fetched from the last CategorizedLine in the sequence)

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
	// load the records into the appropriate border lines
	auto cleaveBegin = sPolyRef->cleaveMap.begin();
	auto cleaveEnd = sPolyRef->cleaveMap.end();
	for (cleaveBegin; cleaveBegin != cleaveEnd; cleaveBegin++)		// cycle through each CleaveSequence
	{
		auto sequenceBegin = cleaveBegin->second.cleavingLines.begin();		// ""
		auto sequenceEnd = cleaveBegin->second.cleavingLines.end();			// ""
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

	// cycle through each border line; if it has any records, determine those record types.
	for (int x = 0; x < sPolyRef->numberOfBorderLines; x++)
	{
		SPolyBorderLines* borderLineRef = &sPolyRef->borderLines[x];
		if (borderLineRef->intersectRecorder.records.size() > 0)	// only do the following if there are actually intersect records in the line.
		{
			std::cout << "Determining record types for border line: " << x << std::endl;
			auto recordsBegin = borderLineRef->intersectRecorder.records.begin();
			auto recordsEnd = borderLineRef->intersectRecorder.records.end();
			for (recordsBegin; recordsBegin != recordsEnd; recordsBegin++)
			{
				recordsBegin->second.determineRecordType();
			}
		}
	}
}