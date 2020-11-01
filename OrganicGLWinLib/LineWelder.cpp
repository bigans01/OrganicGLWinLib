#include "stdafx.h"
#include "LineWelder.h"

void LineWelder::startWelding()
{
	// Step 1: get the first line, in the first cleave sequence, and determine how many border lines it has;
	// in addition, set that beginning cleave sequence to be the SUPER.

	getCleaveSequenceCandidateListMap();	// create the candidates map for each border line, by calling on the SPoly to do it
	getCleaveSequenceMetaTracker();			// builds the meta tracker, so that we may pass it to the NeighboringCleaveSequenceFinder

	std::cout << "####### TESTING:######### cleave map size is: " << sPolyRef->cleaveMap.size() << std::endl;

	auto cleaveBegin = sPolyRef->cleaveMap.begin();						// get an iterator to the first cleave sequence
	beginningSequenceID = cleaveBegin->first;							// set the begin/end cleave sequence ID.
	cleaveBegin->second.setSequenceAsSuper();							// set the CleaveSequence as a SUPER, so the welder knows when to end.
	//auto sequenceBegin = cleaveBegin->second.cleavingLines.begin();
	//auto sequenceEnd = cleaveBegin->second.cleavingLines.rbegin();

	auto sequenceBegin = cleaveBegin->second.cleavingLines.rbegin();
	auto sequenceEnd = cleaveBegin->second.cleavingLines.begin();

	// optional: print out number of lines in each cleave sequence.
	/*
	auto optionalSequenceBegin = sPolyRef->cleaveMap.begin();
	auto optionalSequenceEnd = sPolyRef->cleaveMap.end();
	for (; optionalSequenceBegin != optionalSequenceEnd; optionalSequenceBegin++)
	{
		std::cout << "Sequence [" << optionalSequenceBegin->first << "] has " << optionalSequenceBegin->second.cleavingLines.size() << " lines in it." << std::endl;
	}
	*/

	foundDirection = sequenceBegin->second.direction;
	BorderLinePointPair pointPair;		// point A of this pair is where the welding will begin from; point B of this pair is the beginning point of the WeldedLine, and point A is the end point of the WeldedLine.
	if (sequenceBegin->second.line.numberOfBorderLines == 1)		// if there are multiple CategorizedLines in the cleave sequence, the first line in the sequence
																	// will have exactly one border line. The end of the sequence will have another border line.
	{
		std::cout << "::: Branch hit for one border line... " << std::endl;
		//pointPair = sequenceBegin->second.line.getBorderLinePointPair();
		currentLeadingPoint = sequenceBegin->second.line.getBorderPointFromSingularBorderLineCount();
		currentBorderLineID = sequenceBegin->second.line.getBorderLineIDFromSingularBorderLineCount();		// retrieve the border line to start at, from the first CleavingLine in the first CleavingSequence.
		endingBorderLineID = sequenceEnd->second.line.getBorderLineIDFromSingularBorderLineCount();
		CleaveSequenceMeta* beginningCleaveSequenceMeta = metaTracker.fetchCleaveSequence(beginningSequenceID);		// grab the CleaveSequenceMeta, so that we can retrieve the first line for the welder.
		CategorizedLine currentCategorizedLine = beginningCleaveSequenceMeta->fetchFirstCategorizedLineForWelder();

		//std::cout << "::: First categorized line Aoint A: " << currentCategorizedLine.line.pointA.x << ", " << currentCategorizedLine.line.pointA.y << ", " << currentCategorizedLine.line.pointA.z
															//<< " | Point B: " << currentCategorizedLine.line.pointB.x << ", " << currentCategorizedLine.line.pointB.y << ", " << currentCategorizedLine.line.pointB.z << std::endl;
		std::cout << "::: First categorized line empty normal: " << currentCategorizedLine.emptyNormal.x << ", " << currentCategorizedLine.emptyNormal.y << ", " << currentCategorizedLine.emptyNormal.z << std::endl;
	}
	else if (sequenceBegin->second.line.numberOfBorderLines == 2)	// if there is just one line, it'll have two border lines; now we must
																	// get the borders from the pointA and pointB of this line.
	{
		std::cout << "::: Branch hit for two border lines... " << std::endl;
		BorderLineIDPair retrievedPair = sequenceBegin->second.line.getBorderLineIDPair();
		pointPair = sequenceBegin->second.line.getBorderLinePointPair();
		currentLeadingPoint = pointPair.pointA;			// set the beginning value for the currentLeadingPoint, before welding starts.
		currentBorderLineID = retrievedPair.pointABorderLineID;												// retrieve the border line to start at, from the first CleavingLine in the first CleavingSequence.
		endingBorderLineID = retrievedPair.pointBBorderLineID;
		CleaveSequenceMeta* beginningCleaveSequenceMeta = metaTracker.fetchCleaveSequence(beginningSequenceID);		// grab the CleaveSequenceMeta, so that we can retrieve the first line for the welder.
		CategorizedLine currentCategorizedLine = beginningCleaveSequenceMeta->fetchFirstCategorizedLineForWelder();

		//std::cout << "::: First categorized line Aoint A: " << currentCategorizedLine.line.pointA.x << ", " << currentCategorizedLine.line.pointA.y << ", " << currentCategorizedLine.line.pointA.z
			//<< " | Point B: " << currentCategorizedLine.line.pointB.x << ", " << currentCategorizedLine.line.pointB.y << ", " << currentCategorizedLine.line.pointB.z << std::endl;

		std::cout << "::: First categorized line empty normal: " << currentCategorizedLine.emptyNormal.x << ", " << currentCategorizedLine.emptyNormal.y << ", " << currentCategorizedLine.emptyNormal.z << std::endl;
	}

	std::cout << "******** printing cleave lines in the first sequence: " << std::endl;
	auto testSequenceBegin = cleaveBegin->second.cleavingLines.begin();
	auto testSequenceEnd = cleaveBegin->second.cleavingLines.end();
	for (; testSequenceBegin != testSequenceEnd; testSequenceBegin++)
	{
		std::cout << "Printing line " << testSequenceBegin->first << std::endl;
		std::cout << testSequenceBegin->second.line.pointA.x << ", " << testSequenceBegin->second.line.pointA.y << ", " << testSequenceBegin->second.line.pointA.z << std::endl;
		std::cout << testSequenceBegin->second.line.pointB.x << ", " << testSequenceBegin->second.line.pointB.y << ", " << testSequenceBegin->second.line.pointB.z << std::endl;
		std::cout << "Line, is pointA on border: " << testSequenceBegin->second.line.isPointAOnBorder << ", border value: " << testSequenceBegin->second.line.pointABorder << std::endl;
		std::cout << "Line, is pointB on border: " << testSequenceBegin->second.line.isPointBOnBorder << ", border value: " << testSequenceBegin->second.line.pointBBorder << std::endl;
	}

	// Step 2: find the direction to cycle towards for the currentBorderLineID; i.e., do we start the cycling in the direction of point A or point B of this border line?
	// Remember, if the MassManipulationMode is DESTRUCTION, we must invert the empty normal of the CategorizedLine we are using to find the direction.
	// Also remember, we are going to use COPIES of the border lines and sequences, not the actual refs, when we need to find the CyclingDirection. (may need to change function parameters for this)
	BorderLineCycler lineCycler(sPolyRef);

	// send copies of the border line, and the corresponding categorized line in the CleaveMap that intercepted the border line, to find the direction.
	// store the direction in the LineWelder's foundDirection, so that we can use it throughout the process (should only ever need to find it once, and can be used 
	// without ever having to change it.)
	//foundDirection = lineCycler.findCyclingDirection(sPolyRef->borderLines[currentBorderLineID], currentBorderLineID, sequenceBegin->second, sPolyRef->massManipulationSetting);
	//foundDirection = lineCycler.findCyclingDirection(sPolyRef->borderLines[currentBorderLineID], currentBorderLineID, sequenceBegin->second);

	//getCleaveSequenceCandidateListMap();	// create the candidates map for each border line, by calling on the SPoly to do it
	//getCleaveSequenceMetaTracker();			// builds the meta tracker, so that we may pass it to the NeighboringCleaveSequenceFinder

	// start welding from the first categorized line, in the first cleave, in the border line that the first categorized line is on.
	//currentLeadingPoint = pointPair.pointA;			// set the beginning value for the currentLeadingPoint, before welding starts.//

	//std::cout << "!!! Current pointPair pointA: " << pointPair.pointA.x << ", " << pointPair.pointA.y << ", " << pointPair.pointA.z << std::endl;
	//std::cout << "!!! Current pointPair pointB: " << pointPair.pointB.x << ", " << pointPair.pointB.y << ", " << pointPair.pointB.z << std::endl;
	
	// check border line IDs.
	std::cout << "currentBorderLineID: " << currentBorderLineID << std::endl;
	std::cout << "endingBorderLineID: " << endingBorderLineID << std::endl;

	//	Run in CONTINUE mode until we end up at the endingBorderLineID
	runMode = LineWelderRunMode::CONTINUE;
	CleaveSequenceMeta* beginningCleaveSequenceMeta = metaTracker.fetchCleaveSequence(beginningSequenceID);
	while (currentBorderLineID != endingBorderLineID)
	{
		// pass in: the border line ID, the ID of the CleaveSequence in the CleaveMap, 
		// the ID of the CategorizedLine in the CleaveSequence, the direction
		findRemainingWeldingLines(currentBorderLineID, currentLeadingPoint, &candidateListMap.candidateMap[currentBorderLineID], cleaveBegin->first);

		std::cout << "##!!! Iterating while... " << std::endl;
		int someVal = 5;
		std::cin >> someVal;

	}

	//	Run in ENDING mode when we are on endingBorderLineID, and continue to find remaining lines.
	std::cout << "::> Number of remaining lines in the beginning cleave sequence is: " << beginningCleaveSequenceMeta->numberOfRemainingLines << std::endl;
	
	runMode = LineWelderRunMode::ENDING;
	while (beginningCleaveSequenceMeta->numberOfRemainingLines > 0)
	{
		findRemainingWeldingLines(currentBorderLineID, currentLeadingPoint, &candidateListMap.candidateMap[currentBorderLineID], cleaveBegin->first);
		std::cout << "##!!! Iterating ENDING while... " << std::endl;
		int someVal = 5;
		std::cin >> someVal;
	}
	

	std::cout << "### ............................................................................>>>>>>>>>>>>>> Welding complete, enter number to continue to next poly. " << std::endl;
	int someVal = 3;
	std::cin >> someVal;
	// use the unused point of the categorized line to determine how to quat to Z = 0 (Z-planar).
}

void LineWelder::getCleaveSequenceCandidateListMap()
{
	candidateListMap = sPolyRef->buildCleaveSequenceCandidateListMap();
}

void LineWelder::getCleaveSequenceMetaTracker()
{
	metaTracker = sPolyRef->buildCleaveSequenceMetaTracker();
}

void LineWelder::findRemainingWeldingLines(int in_currentBorderLineID, glm::vec3 in_leadingPoint, CleaveSequenceCandidateList* in_cleaveSequenceCandidateListRef, int in_finderStartingCleaveSequenceID)
{
	std::cout << "----------------------------------> Welding leading point for this line is: " << in_leadingPoint.x << ", " << in_leadingPoint.y << ", " << in_leadingPoint.z << std::endl;
	// find any neighboring cleave lines that exist on the same border line, if they exist. If they do exist, fetch the next
	// pass in these parameters:
	// -the border line that the finder needs to run on
	// -a reference to border line that the finder will run on
	// -a referenece to the SPoly's cleave map
	// -the CyclingDirection the LineWelder is running in
	// -a reference to a set indicating the candidates (that is, selectable CleaveSequences that haven't been consumed/used) that exist on the border line that the finder runs on
	// -the ID of the current CleaveSequence this finder will start from

	std::cout << "====> Current border line ID is: " << in_currentBorderLineID << std::endl;
	std::cout << "====> Current border line planar vector is: " << sPolyRef->borderLines[in_currentBorderLineID].planarVector.x << ", " << sPolyRef->borderLines[in_currentBorderLineID].planarVector.y << ", " << sPolyRef->borderLines[in_currentBorderLineID].planarVector.z << std::endl;

	//	// get a ref to the intersect recorder in our current border line.
	BorderLineIntersectRecorder* intersectRecorderRef = &sPolyRef->borderLines[in_currentBorderLineID].intersectRecorder;
	int nextBorderLineID = 0;	// will be set by conditions below...
	if (intersectRecorderRef->records.size() > 1)	// only do this check if there are MULTIPLE CleaveSequences in a BorderLine
	{
		NeighboringCleaveSequenceFinder nextCleaveSequenceFinder(in_currentBorderLineID, 
																&sPolyRef->borderLines[in_currentBorderLineID], 
																&sPolyRef->cleaveMap, foundDirection, 
																in_cleaveSequenceCandidateListRef, 
																in_finderStartingCleaveSequenceID, 
																&metaTracker, 
																in_leadingPoint, 
																runMode, 
																&weldedLines);
		if (nextCleaveSequenceFinder.wereNeighborsFound() == true)
		{
			FoundCleaveSequence discoveredSequence = nextCleaveSequenceFinder.getSelectedCleaveSequenceMeta();
			std::cout << "||||||||| neighboring cleave sequence was found, stats are: " << std::endl;
			std::cout << "| cleave sequence ID: " << discoveredSequence.cleaveSequenceID << std::endl;
			std::cout << "| distance:           " << discoveredSequence.distance << std::endl;
			std::cout << "| point:              " << discoveredSequence.cleaveSequenceTracingBeginPoint.x << ", " << discoveredSequence.cleaveSequenceTracingBeginPoint.y << ", " << discoveredSequence.cleaveSequenceTracingBeginPoint.z << std::endl;

			CleaveSequenceMeta* fetchedCleaveSequenceMeta = metaTracker.fetchCleaveSequence(discoveredSequence.cleaveSequenceID);
			currentHierarchyPositionOfLatestCleaveSequence = fetchedCleaveSequenceMeta->cleaveSequencePtr->hierarchyPosition;			// update the current hierarchy position to be the value of the neighboring CleaveSequence we just found.
			fetchedCleaveSequenceMeta->determineCrawlDirectionFromPoint(discoveredSequence.cleaveSequenceTracingBeginPoint);			// for the cleave sequence we found, determine the the direction we'll go in.
			//numberOfRemainingLines
			//int numberOfLinesToCrawl = fetchedCleaveSequenceMeta->numberOfLines;
			int numberOfLinesToCrawl = fetchedCleaveSequenceMeta->numberOfRemainingLines;
			for (int x = 0; x < numberOfLinesToCrawl; x++)
			{
				std::cout << "| Crawling line... " << std::endl;
				CategorizedLine currentCategorizedLine = fetchedCleaveSequenceMeta->fetchNextCategorizedLineInSequence();

				std::cout << "| Current line, point A: " << currentCategorizedLine.line.pointA.x << ", " << currentCategorizedLine.line.pointA.y << ", " << currentCategorizedLine.line.pointA.z << std::endl;
				std::cout << "| Current line, point B: " << currentCategorizedLine.line.pointB.x << ", " << currentCategorizedLine.line.pointB.y << ", " << currentCategorizedLine.line.pointB.z << std::endl;

				// will need to do this with the lin here, such as making into a CrawlableLine
				// ...
				// ...
				//currentLeadingPoint = currentCategorizedLine.line.pointB;		// hard-coded test, remove later...
				currentLeadingPoint = currentCategorizedLine.line.fetchNextPointBasedOnCyclingDirection(foundDirection);
				std::cout << "| Current leading point, as a result of crawling, is: " << currentLeadingPoint.x << ", " << currentLeadingPoint.y << ", " << currentLeadingPoint.z << std::endl;

				// for the last iteration in this loop, get the next border line ID from the final categorized line in the sequence,
				// so the welder knows where to start in the next iteration.
				if (x == (numberOfLinesToCrawl - 1))
				{
					nextBorderLineID = fetchedCleaveSequenceMeta->acquireNextBorderLineID();
					std::cout << "##!!!!! Next border line ID will be: " << nextBorderLineID << std::endl;
				}
			}

			// check if the CleaveSequence has run all it's lines; if it has, we must remove it from the appropriate CleaveSequenceCandidateList.
			bool isCleaveSequenceComplete = fetchedCleaveSequenceMeta->checkIfCleaveSequenceHasRunallCategorizedLines();
			if (isCleaveSequenceComplete == true)
			{
				std::cout << "######### CleaveSequence is COMPLETE, removing from candidateList, the ID of: " << discoveredSequence.cleaveSequenceID << std::endl;
				//in_cleaveSequenceCandidateListRef->removeCandidate(discoveredSequence.cleaveSequenceID);
				//candidateListMap.candidateMap[currentBorderLineID] = (*in_cleaveSequenceCandidateListRef);
				candidateListMap.removeCandidateFromAllCandidateLists(discoveredSequence.cleaveSequenceID);
			}


		}
		else if (nextCleaveSequenceFinder.wereNeighborsFound() == false)
		{
			std::cout << "!!! No neighbors found, from the leading point: " << currentLeadingPoint.x << ", " << currentLeadingPoint.y << ", " << currentLeadingPoint.z << std::endl;
			nextBorderLineID = sPolyRef->getNextBorderLineID(currentBorderLineID, foundDirection);
			currentLeadingPoint = sPolyRef->getBorderLineEndpoint(currentBorderLineID, foundDirection);
			std::cout << "Next border LINE id will be: " << nextBorderLineID << std::endl;
			std::cout << "Leading point is now: " << currentLeadingPoint.x << ", " << currentLeadingPoint.y << ", " << currentLeadingPoint.z << std::endl;
		}
		currentBorderLineID = nextBorderLineID;
	}
	else if (intersectRecorderRef->records.size() == 1)		// only do this if there is ONE CleaveSequence in the current border line.
	{
		std::cout << "::: Note: border Line with ID: " << currentBorderLineID << " has only 1 intercept record. " << std::endl;
		NeighboringCleaveSequenceFinder nextCleaveSequenceFinder(in_currentBorderLineID, 
																&sPolyRef->borderLines[in_currentBorderLineID], 
																&sPolyRef->cleaveMap, 
																foundDirection, 
																in_cleaveSequenceCandidateListRef, 
																in_finderStartingCleaveSequenceID,
																&metaTracker, 
																in_leadingPoint, 
																runMode, 
																&weldedLines);
		if (nextCleaveSequenceFinder.wereNeighborsFound() == true)
		{
			FoundCleaveSequence discoveredSequence = nextCleaveSequenceFinder.getSelectedCleaveSequenceMeta();

			CleaveSequenceMeta* fetchedCleaveSequenceMeta = metaTracker.fetchCleaveSequence(discoveredSequence.cleaveSequenceID);
			currentHierarchyPositionOfLatestCleaveSequence = fetchedCleaveSequenceMeta->cleaveSequencePtr->hierarchyPosition;			// update the current hierarchy position to be the value of the neighboring CleaveSequence we just found.
			fetchedCleaveSequenceMeta->determineCrawlDirectionFromPoint(discoveredSequence.cleaveSequenceTracingBeginPoint);			// for the cleave sequence we found, determine the the direction we'll go in.
			//numberOfRemainingLines
			//int numberOfLinesToCrawl = fetchedCleaveSequenceMeta->numberOfLines;
			int numberOfLinesToCrawl = fetchedCleaveSequenceMeta->numberOfRemainingLines;

			std::cout << "!!! Number of lines to crawl: " << numberOfLinesToCrawl << std::endl;

			for (int x = 0; x < numberOfLinesToCrawl; x++)
			{
				std::cout << "| Crawling line... " << std::endl;
				CategorizedLine currentCategorizedLine = fetchedCleaveSequenceMeta->fetchNextCategorizedLineInSequence();
				// will need to do this with the lin here, such as making into a CrawlableLine
				// ...
				// ...
				//currentLeadingPoint = currentCategorizedLine.line.pointB;		// hard-coded test, remove later...
				currentLeadingPoint = currentCategorizedLine.line.fetchNextPointBasedOnCyclingDirection(foundDirection);
				std::cout << "| Current leading point, as a result of crawling, is: " << currentLeadingPoint.x << ", " << currentLeadingPoint.y << ", " << currentLeadingPoint.z << std::endl;

				// for the last iteration in this loop, get the next border line ID from the final categorized line in the sequence,
				// so the welder knows where to start in the next iteration.
				if (x == (numberOfLinesToCrawl - 1))
				{
					nextBorderLineID = fetchedCleaveSequenceMeta->acquireNextBorderLineID();
					std::cout << "##!!!!! Next border line ID will be: " << nextBorderLineID << std::endl;
				}
			}

			// check if the CleaveSequence has run all it's lines; if it has, we must remove it from the appropriate CleaveSequenceCandidateList.
			bool isCleaveSequenceComplete = fetchedCleaveSequenceMeta->checkIfCleaveSequenceHasRunallCategorizedLines();
			if (isCleaveSequenceComplete == true)
			{
				std::cout << "######### CleaveSequence is COMPLETE, removing from candidateList, the ID of: " << discoveredSequence.cleaveSequenceID << std::endl;
				//in_cleaveSequenceCandidateListRef->removeCandidate(discoveredSequence.cleaveSequenceID);
				//candidateListMap.candidateMap[currentBorderLineID] = (*in_cleaveSequenceCandidateListRef);
				candidateListMap.removeCandidateFromAllCandidateLists(discoveredSequence.cleaveSequenceID);
			}
			currentBorderLineID = nextBorderLineID;
		}
		else if (nextCleaveSequenceFinder.wereNeighborsFound() == false)
		{
			std::cout << "!!! The single intercept record was already consumed!!! " << currentLeadingPoint.x << ", " << currentLeadingPoint.y << ", " << currentLeadingPoint.z << std::endl;
			nextBorderLineID = sPolyRef->getNextBorderLineID(currentBorderLineID, foundDirection);
			currentLeadingPoint = sPolyRef->getBorderLineEndpoint(currentBorderLineID, foundDirection);
			std::cout << "Next border LINE id will be: " << nextBorderLineID << std::endl;
			std::cout << "Leading point is now: " << currentLeadingPoint.x << ", " << currentLeadingPoint.y << ", " << currentLeadingPoint.z << std::endl;
		}
		currentBorderLineID = nextBorderLineID;
	}
	else if (intersectRecorderRef->records.size() == 0)		// only do this if there are ZERO CleaveSequences in the current border line.
	{
		// foundDirection
		int nextBorderLineID = sPolyRef->getNextBorderLineID(currentBorderLineID, foundDirection);
		std::cout << ":: This border line has no records; producing line and proceeding to next border line with ID: " << nextBorderLineID << std::endl;
		currentLeadingPoint = sPolyRef->getBorderLineEndpoint(currentBorderLineID, foundDirection);
		std::cout << "Next border LINE id will be: " << nextBorderLineID << std::endl;
		std::cout << "Leading point is now: " << currentLeadingPoint.x << ", " << currentLeadingPoint.y << ", " << currentLeadingPoint.z << std::endl;

		currentBorderLineID = nextBorderLineID;

	}
}