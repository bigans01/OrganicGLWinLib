#include "stdafx.h"
#include "CleaveSequenceIntersectFinder.h"

CleaveSequenceIntersectFinder::CleaveSequenceIntersectFinder(int in_originalPolyID, SPoly* in_sPolyRef, PolyDebugLevel in_polyDebugLevel)
{
	intersectFinderLoggerDebugLevel = in_polyDebugLevel;
	intersectFinderLogger.setDebugLevel(in_polyDebugLevel);

	sPolyRef = in_sPolyRef;
	originalPolyID = in_originalPolyID;
	triangleSupergroup.setOriginalSPolyID(in_originalPolyID);
	int cleaveMapSize = int(in_sPolyRef->cleaveMap.size());
	
	// only perform the check if the size is >= 1; we don't need to do any work if there aren't any cleave sequences.
	
	if (cleaveMapSize >= 1)
	{
		//std::cout << "#- Records              > CleaveSequences found in cleaveMap; inserting intercept records into appropriate border lines...  " << cleaveMapSize << std::endl;

		SPolyBorderLines* borderLineRef = &sPolyRef->borderLines[0];

		//std::cout << "#--- Printing out begin and end points of first border line:" << std::endl; 
		//std::cout << "Point A: " << sPolyRef->borderLines[0].pointA.x << ", " << sPolyRef->borderLines[0].pointA.y << ", " << sPolyRef->borderLines[0].pointA.z << std::endl;
		//std::cout << "Point B: " << sPolyRef->borderLines[0].pointB.x << ", " << sPolyRef->borderLines[0].pointB.y << ", " << sPolyRef->borderLines[0].pointB.z << std::endl;

		auto truestart = std::chrono::high_resolution_clock::now();
		loadInterceptRecords();		// load the data about the CleaveSequence's categorized lines, and the border lines they intercept, into the appropriate border line's BorderLineInterceptRecorders.


		//printCurrentLineValuesInCleaveSequences();
		//std::cout << "## Finished printing current cleave sequence line values..." << std::endl;
		//int sillyVal7 = 3;
		//std::cin >> sillyVal7;
		if (intersectFinderLogger.isLoggingSet() == true)
		{
			intersectFinderLogger.log("(CleaveSequenceIntersectFinder) >>>> starting printCurrentLineValuesInCleaveSequences()", "\n");
			printCurrentLineValuesInCleaveSequences();
			intersectFinderLogger.log("(CleaveSequenceIntersectFinder) >>>> finished printCurrentLineValuesInCleaveSequences()", "\n");
			intersectFinderLogger.waitForDebugInput();
		}

		auto trueend = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> trueelapsed2 = trueend - truestart;
		//std::cout << "#-> (Intercept records ) record load Time !!  > " << std::fixed << trueelapsed2.count() << std::endl;
		
		/*
		std::cout << "#--- Printing out begin and end points of first border line (pre-LineWelder):" << std::endl;
		std::cout << "Point A: " << sPolyRef->borderLines[0].pointA.x << ", " << sPolyRef->borderLines[0].pointA.y << ", " << sPolyRef->borderLines[0].pointA.z << std::endl;
		std::cout << "Point B: " << sPolyRef->borderLines[0].pointB.x << ", " << sPolyRef->borderLines[0].pointB.y << ", " << sPolyRef->borderLines[0].pointB.z << std::endl;

		std::cout << "#--- Printing out begin and end points of second border line (pre-LineWelder):" << std::endl;
		std::cout << "Point A: " << sPolyRef->borderLines[1].pointA.x << ", " << sPolyRef->borderLines[1].pointA.y << ", " << sPolyRef->borderLines[1].pointA.z << std::endl;
		std::cout << "Point B: " << sPolyRef->borderLines[1].pointB.x << ", " << sPolyRef->borderLines[1].pointB.y << ", " << sPolyRef->borderLines[1].pointB.z << std::endl;
		*/

		// weld the lines; put the results into the line pool. Then, put it into the welded triangle builder.
		LineWelder welder(in_sPolyRef);		
		while (welder.getRemainingCandidateCount() > 0)
		{
			welder.startWelding();							// perform one welding run
			linePool = welder.retrieveLinePool();			
			linePool.printLines();

			std::cout << "!!! Finished printing lines, continue? " << std::endl;
			int finishedPrintLines = 3;
			std::cin >> finishedPrintLines;

			WeldedTriangleGroupBuilder groupBuilder;
			groupBuilder.setWeldedLinePool(linePool);
			groupBuilder.runTracingObservers();
			//weldedTriangles = std::move(groupBuilder.weldedTriangleVector);
			//triangleSupergroup.insertTriangleContainer(std::move(groupBuilder.weldedTriangleVector));

			auto containerVectorBegin = groupBuilder.weldedTriangleContainerVector.begin();
			auto containerVectorEnd = groupBuilder.weldedTriangleContainerVector.end();
			for (; containerVectorBegin != containerVectorEnd; containerVectorBegin++)
			{
				triangleSupergroup.insertTriangleContainer(std::move(*containerVectorBegin));
			}


			welder.clearLinePool();

			//std::cout << ":::: Welding iteration complete; enter value to continue. " << std::endl;
			//int someVal = 5;
			//std::cin >> someVal;
		}

		triangleSupergroup.printPointsInSupergroup();
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
			//std::cout << "!! Cycling through cleaveSequence..." << std::endl;
			//std::cout << "!! number of border lines is: " << sequenceBegin->second.line.numberOfBorderLines << std::endl;
			intersectFinderLogger.log("(CleaveSequenceIntersectFinder) !! Cycling through cleaveSequence...", "\n");
			intersectFinderLogger.log("(CleaveSequenceIntersectFinder) !!number of border lines is : ", "\n");


			/*
			if (sequenceBegin->second.type == IntersectionType::INTERCEPTS_POINT_PRECISE)
			{
				std::cout << "!!! NOTICE: this is an INTERCEPTS_POINT_PRECISE. " << std::endl;
			}
			*/

			if (sequenceBegin->second.line.numberOfBorderLines == 1)		// it's a PARTIAL_BOUND; if it contains one interception, load the data into appropriate border line's BorderLineIntersectRecorder
			{
				int borderLineId = sequenceBegin->second.line.getBorderLineIDFromSingularBorderLineCount();		// get the appropriate border line to insert data into.
				SPolyBorderLines* borderLineRef = &sPolyRef->borderLines[borderLineId];								// get the line ref.
				borderLineRef->intersectRecorder.insertNewRecord(cleaveBegin->first, sequenceBegin->first, &sequenceBegin->second);		// first argument: the ID of the cleave sequence
																																		// second argument: the ID of the categorized line we're inserting from the sequence
																																		// third argument: a reference to the categorized line itself
				//std::cout << "## BorderLine updated, BorderLineID: " << borderLineId << " SequenceID: " << cleaveBegin->first << " | CategorizedLineID: " << sequenceBegin->first << std::endl;
				intersectFinderLogger.log("(CleaveSequenceIntersectFinder) ## BorderLine updated, BorderLineID: ", borderLineId, " SequenceID: ", cleaveBegin->first, " | CategorizedLineID: ", sequenceBegin->first, "\n");
			}	
			else if (sequenceBegin->second.line.numberOfBorderLines == 2)		// it's an A_SLICE
			{
				//std::cout << "## Number of border lines is 2!" << std::endl;
				intersectFinderLogger.log("(CleaveSequenceIntersectFinder) ## Number of border lines is 2! ", "\n");

				int borderLineAId = sequenceBegin->second.line.pointABorder;
				SPolyBorderLines* borderLineARef = &sPolyRef->borderLines[borderLineAId];
				borderLineARef->intersectRecorder.insertNewRecord(cleaveBegin->first, sequenceBegin->first, &sequenceBegin->second);

				int borderLineBId = sequenceBegin->second.line.pointBBorder;
				SPolyBorderLines* borderLineBRef = &sPolyRef->borderLines[borderLineBId];
				borderLineBRef->intersectRecorder.insertNewRecord(cleaveBegin->first, sequenceBegin->first, &sequenceBegin->second);

			}
		}

	}

	// cycle through each border line; if it has any records, determine those record types.
	
	for (int x = 0; x < sPolyRef->numberOfBorderLines; x++)
	{
		SPolyBorderLines* borderLineRef = &sPolyRef->borderLines[x];
		if (borderLineRef->intersectRecorder.records.size() > 0)	// only do the following if there are actually intersect records in the line.
		{
			//std::cout << "Determining record types for border line: " << x << std::endl;
			auto recordsBegin = borderLineRef->intersectRecorder.records.begin();
			auto recordsEnd = borderLineRef->intersectRecorder.records.end();
			for (recordsBegin; recordsBegin != recordsEnd; recordsBegin++)
			{
				recordsBegin->second.determineRecordType();
			}
		}
	}
	
}

void CleaveSequenceIntersectFinder::printCurrentLineValuesInCleaveSequences()
{
	// load the records into the appropriate border lines
	auto cleaveBegin = sPolyRef->cleaveMap.begin();
	auto cleaveEnd = sPolyRef->cleaveMap.end();
	for (cleaveBegin; cleaveBegin != cleaveEnd; cleaveBegin++)		// cycle through each CleaveSequence
	{
		std::cout << ">>> Sequence: " << cleaveBegin->first << std::endl;
		auto sequenceBegin = cleaveBegin->second.cleavingLines.begin();		// ""
		auto sequenceEnd = cleaveBegin->second.cleavingLines.end();			// ""
		for (sequenceBegin; sequenceBegin != sequenceEnd; sequenceBegin++)	// cycle through each CategorizedLine in each CleaveSequence
		{
			std::cout << "Current line in sequence: "
				<< "point A-> " << sequenceBegin->second.line.pointA.x << ", " << sequenceBegin->second.line.pointA.y << ", " << sequenceBegin->second.line.pointA.z
				<< "point B-> " << sequenceBegin->second.line.pointB.x << ", " << sequenceBegin->second.line.pointB.y << ", " << sequenceBegin->second.line.pointB.z << std::endl;
		}

	}
}