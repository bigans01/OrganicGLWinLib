#include "stdafx.h"
#include "LineWelder.h"

WeldedLinePool LineWelder::retrieveLinePool()
{
	return weldedLines;
}

void LineWelder::insertNewWeldingLine(glm::vec3 in_pointA, glm::vec3 in_pointB, glm::vec3 in_emptyNormal)
{
	WeldedLine beginningWeldedLine(in_pointA, in_pointB, in_emptyNormal);
	weldedLines.insertLineIntoPool(beginningWeldedLine);
}

void LineWelder::updateLeadingPointAndInsertNewWeldingLineFromBorderLineData()
{
	glm::vec3 newLineBeginPoint = currentLeadingPoint;
	glm::vec3 newLineEmptyNormal = sPolyRef->borderLines[currentBorderLineID].planarVector;
	currentLeadingPoint = sPolyRef->getBorderLineEndpoint(currentBorderLineID, foundDirection);
	insertNewWeldingLine(newLineBeginPoint, currentLeadingPoint, newLineEmptyNormal);
}

int LineWelder::getRemainingCandidateCount()
{
	return candidateListMap.remainingCandidateCount;
}

void LineWelder::clearLinePool()
{
	weldedLines.clearPool();
}

void LineWelder::startWelding()
{
	runMode = LineWelderRunMode::CONTINUE; // reset the value to CONTINUE.
	// Step 1: get the first line, in the first cleave sequence, and determine how many border lines it has;
	// in addition, set that beginning cleave sequence to be the SUPER.

	//getCleaveSequenceCandidateListMap();	// create the candidates map for each border line, by calling on the SPoly to do it
	//getCleaveSequenceMetaTracker();			// builds the meta tracker, so that we may pass it to the NextCleaveSequenceFinder

	std::cout << "####### TESTING:######### cleave map size is: " << sPolyRef->cleaveMap.size() << std::endl;

	//auto cleaveBegin = sPolyRef->cleaveMap.begin();						

	int nextAvailableCandidate = candidateListMap.fetchNextAvailableCandidate();
	auto cleaveBegin = sPolyRef->cleaveMap.find(nextAvailableCandidate);

	beginningSequenceID = cleaveBegin->first;							
	//cleaveBegin->second.setSequenceAsSuper();							// set the CleaveSequence as a SUPER, so the welder knows when to end.

	auto startingCategorizedLine = cleaveBegin->second.cleavingLines.rbegin();
	auto endingCategorizedLine = cleaveBegin->second.cleavingLines.begin();

	std::cout << ":::::::: Values of rbegin categorized line are: " << std::endl;
	std::cout << "Point A: " << startingCategorizedLine->second.line.pointA.x << ", " << startingCategorizedLine->second.line.pointA.y << ", " << startingCategorizedLine->second.line.pointA.z << std::endl;
	std::cout << "Point B: " << startingCategorizedLine->second.line.pointB.x << ", " << startingCategorizedLine->second.line.pointB.y << ", " << startingCategorizedLine->second.line.pointB.z << std::endl;
	std::cout << "Empty normal: " << startingCategorizedLine->second.emptyNormal.x << ", " << startingCategorizedLine->second.emptyNormal.y << ", " << startingCategorizedLine->second.emptyNormal.z << std::endl;

	std::cout << ":::::::: Values of begin categorized line are: " << std::endl;
	std::cout << "Point A: " << endingCategorizedLine->second.line.pointA.x << ", " << endingCategorizedLine->second.line.pointA.y << ", " << endingCategorizedLine->second.line.pointA.z << std::endl;
	std::cout << "Point B: " << endingCategorizedLine->second.line.pointB.x << ", " << endingCategorizedLine->second.line.pointB.y << ", " << endingCategorizedLine->second.line.pointB.z << std::endl;
	std::cout << "Empty normal: " << endingCategorizedLine->second.emptyNormal.x << ", " << endingCategorizedLine->second.emptyNormal.y << ", " << endingCategorizedLine->second.emptyNormal.z << std::endl;



	// optional: print out number of lines in each cleave sequence.
	/*
	auto optionalstartingCategorizedLine = sPolyRef->cleaveMap.begin();
	auto optionalendingCategorizedLine = sPolyRef->cleaveMap.end();
	for (; optionalstartingCategorizedLine != optionalendingCategorizedLine; optionalstartingCategorizedLine++)
	{
		std::cout << "Sequence [" << optionalstartingCategorizedLine->first << "] has " << optionalstartingCategorizedLine->second.cleavingLines.size() << " lines in it." << std::endl;
	}
	*/
	CategorizedLine currentCategorizedLine;
	foundDirection = startingCategorizedLine->second.direction;
	BorderLinePointPair pointPair;		// point A of this pair is where the welding will begin from; point B of this pair is the beginning point of the WeldedLine, and point A is the end point of the WeldedLine.
	if (startingCategorizedLine->second.line.numberOfBorderLines == 1)		// if there are multiple CategorizedLines in the cleave sequence, the first line in the sequence
																	// will have exactly one border line. The end of the sequence will have another border line.
	{
		std::cout << "::: Branch hit for one border line... " << std::endl;
		//pointPair = startingCategorizedLine->second.line.getBorderLinePointPair();
		currentLeadingPoint = startingCategorizedLine->second.line.getBorderPointFromSingularBorderLineCount();
		currentBorderLineID = startingCategorizedLine->second.line.getBorderLineIDFromSingularBorderLineCount();		
		endingBorderLineID = endingCategorizedLine->second.line.getBorderLineIDFromSingularBorderLineCount();
		CleaveSequenceMeta* beginningCleaveSequenceMeta = metaTracker.fetchCleaveSequence(beginningSequenceID);	
		currentCategorizedLine = beginningCleaveSequenceMeta->fetchFirstCategorizedLineForWelder();

		std::cout << "::: First categorized line Aoint A: " << currentCategorizedLine.line.pointA.x << ", " << currentCategorizedLine.line.pointA.y << ", " << currentCategorizedLine.line.pointA.z
															<< " | Point B: " << currentCategorizedLine.line.pointB.x << ", " << currentCategorizedLine.line.pointB.y << ", " << currentCategorizedLine.line.pointB.z << std::endl;
		std::cout << "::: First categorized line empty normal: " << currentCategorizedLine.emptyNormal.x << ", " << currentCategorizedLine.emptyNormal.y << ", " << currentCategorizedLine.emptyNormal.z << std::endl;
	}
	else if (startingCategorizedLine->second.line.numberOfBorderLines == 2)	// if there is just one line, it'll have two border lines; now we must
																			// get the borders from the pointA and pointB of this line.
	{
		std::cout << "::: Branch hit for two border lines... " << std::endl;
		BorderLineIDPair retrievedPair = startingCategorizedLine->second.line.getBorderLineIDPair();
		pointPair = startingCategorizedLine->second.line.getBorderLinePointPair();
		currentLeadingPoint = pointPair.pointA;			
		currentBorderLineID = retrievedPair.pointABorderLineID;											
		endingBorderLineID = retrievedPair.pointBBorderLineID;
		CleaveSequenceMeta* beginningCleaveSequenceMeta = metaTracker.fetchCleaveSequence(beginningSequenceID);		
		currentCategorizedLine = beginningCleaveSequenceMeta->fetchFirstCategorizedLineForWelder();

		//std::cout << "::: First categorized line Aoint A: " << currentCategorizedLine.line.pointA.x << ", " << currentCategorizedLine.line.pointA.y << ", " << currentCategorizedLine.line.pointA.z
			//<< " | Point B: " << currentCategorizedLine.line.pointB.x << ", " << currentCategorizedLine.line.pointB.y << ", " << currentCategorizedLine.line.pointB.z << std::endl;


		//std::cout << "::: First categorized line empty normal: " << currentCategorizedLine.emptyNormal.x << ", " << currentCategorizedLine.emptyNormal.y << ", " << currentCategorizedLine.emptyNormal.z << std::endl;
	}

	// insert the very first line of the welding pool.
	WeldedLine beginningWeldedLine(currentCategorizedLine.line.pointA, currentCategorizedLine.line.pointB, currentCategorizedLine.emptyNormal);
	weldedLines.insertLineIntoPool(beginningWeldedLine);

	/*
	std::cout << "******** printing cleave lines in the first sequence: " << std::endl;
	auto testSequenceBegin = cleaveBegin->second.cleavingLines.begin();
	auto testendingCategorizedLine = cleaveBegin->second.cleavingLines.end();
	for (; testSequenceBegin != testendingCategorizedLine; testSequenceBegin++)
	{
		std::cout << "Printing line " << testSequenceBegin->first << std::endl;
		std::cout << testSequenceBegin->second.line.pointA.x << ", " << testSequenceBegin->second.line.pointA.y << ", " << testSequenceBegin->second.line.pointA.z << std::endl;
		std::cout << testSequenceBegin->second.line.pointB.x << ", " << testSequenceBegin->second.line.pointB.y << ", " << testSequenceBegin->second.line.pointB.z << std::endl;
		std::cout << "Line, is pointA on border: " << testSequenceBegin->second.line.isPointAOnBorder << ", border value: " << testSequenceBegin->second.line.pointABorder << std::endl;
		std::cout << "Line, is pointB on border: " << testSequenceBegin->second.line.isPointBOnBorder << ", border value: " << testSequenceBegin->second.line.pointBBorder << std::endl;
	}
	*/

	//std::cout << "!!! Current pointPair pointA: " << pointPair.pointA.x << ", " << pointPair.pointA.y << ", " << pointPair.pointA.z << std::endl;
	//std::cout << "!!! Current pointPair pointB: " << pointPair.pointB.x << ", " << pointPair.pointB.y << ", " << pointPair.pointB.z << std::endl;
	
	// check border line IDs.
	//std::cout << "currentBorderLineID: " << currentBorderLineID << std::endl;
	//std::cout << "endingBorderLineID: " << endingBorderLineID << std::endl;

	//	Run in CONTINUE mode until we end up at the endingBorderLineID
	runMode = LineWelderRunMode::CONTINUE;
	CleaveSequenceMeta* beginningCleaveSequenceMeta = metaTracker.fetchCleaveSequence(beginningSequenceID);
	if (currentBorderLineID != endingBorderLineID)		// only do this, if the first sequence doesn't start AND end on the same border line.
	{
		while (currentBorderLineID != endingBorderLineID)
		{
			// pass in: the border line ID, the ID of the CleaveSequence in the CleaveMap, 
			// the ID of the CategorizedLine in the CleaveSequence, the direction

			std::cout << "####  Begin and end border line differ..." << std::endl;
			findRemainingWeldingLines(currentBorderLineID, currentLeadingPoint, &candidateListMap.candidateMap[currentBorderLineID], cleaveBegin->first);

			//std::cout << "##!!! Iterating while... " << std::endl;
			//int someVal = 5;
			//std::cin >> someVal;

		}


		//	Run in ENDING mode when we are on endingBorderLineID, and continue to find remaining lines.
		std::cout << "::> Number of remaining lines in the beginning cleave sequence is: " << beginningCleaveSequenceMeta->numberOfRemainingLines << std::endl;
		
	}

	else if (currentBorderLineID == endingBorderLineID)		// the cleave sequence begins and ends on the same border line
	{
		runMode = LineWelderRunMode::ENDING;
		std::cout << "############# SPECIAL LOGIC NEEDED. " << std::endl;
		if (sPolyRef->massManipulationSetting == MassManipulationMode::DESTRUCTION)		// this function should only ever be called when in destruction mode;
																						// the plan for this logic needs to be reworked a bit.
		{
			findRemainingWeldingLinesClosedCircuit(currentBorderLineID, currentLeadingPoint, &candidateListMap.candidateMap[currentBorderLineID], cleaveBegin->first);
		}
		else if (sPolyRef->massManipulationSetting == MassManipulationMode::CREATION)
		{
			findRemainingWeldingLines(currentBorderLineID, currentLeadingPoint, &candidateListMap.candidateMap[currentBorderLineID], cleaveBegin->first);
		}
	}


	runMode = LineWelderRunMode::ENDING;
	while (beginningCleaveSequenceMeta->numberOfRemainingLines > 0)
	{
		if (currentBorderLineID != endingBorderLineID)		// only do this, if the first sequence doesn't start AND end on the same border line.
		{
			findRemainingWeldingLines(currentBorderLineID, currentLeadingPoint, &candidateListMap.candidateMap[currentBorderLineID], cleaveBegin->first);
		}
		else if (currentBorderLineID == endingBorderLineID)		// the cleave sequence begins and ends on the same border line
		{
			//runMode = LineWelderRunMode::ENDING;
			//std::cout << "############# SPECIAL LOGIC NEEDED. " << std::endl;
			if (sPolyRef->massManipulationSetting == MassManipulationMode::DESTRUCTION)		// this function should only ever be called when in destruction mode;
																							// the plan for this logic needs to be reworked a bit.
			{
				findRemainingWeldingLinesClosedCircuit(currentBorderLineID, currentLeadingPoint, &candidateListMap.candidateMap[currentBorderLineID], cleaveBegin->first);
			}
			else if (sPolyRef->massManipulationSetting == MassManipulationMode::CREATION)
			{
				findRemainingWeldingLines(currentBorderLineID, currentLeadingPoint, &candidateListMap.candidateMap[currentBorderLineID], cleaveBegin->first);
			}
		}
		//std::cout << "##!!! Iterating ENDING while... " << std::endl;
		//int someVal = 5;
		//std::cin >> someVal;
	}


	/*
	}
	else if (currentBorderLineID == endingBorderLineID)
	{
		runMode = LineWelderRunMode::ENDING;
		std::cout << "############# SPECIAL LOGIC NEEDED. " << std::endl;
	}
	*/
	
	candidateListMap.printCandidateLists();
	candidateListMap.updateRemainingCandidateCount();

	std::cout << "### ............................................................................>>>>>>>>>>>>>> Welding complete; lines are ready for WeldedTriangleGroupBuilder. Enter number to continue. " << std::endl;
	int someVal = 3;
	std::cin >> someVal;
	// use the unused point of the categorized line to determine how to quat to Z = 0 (Z-planar).
}

void LineWelder::getCleaveSequenceCandidateListMap()
{
	candidateListMap = sPolyRef->buildCleaveSequenceCandidateListMap();
	candidateListMap.updateRemainingCandidateCount();
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
		NextCleaveSequenceFinder nextCleaveSequenceFinder(in_currentBorderLineID, 
																&sPolyRef->borderLines[in_currentBorderLineID], 
																&sPolyRef->cleaveMap, foundDirection, 
																in_cleaveSequenceCandidateListRef, 
																in_finderStartingCleaveSequenceID, 
																&metaTracker, 
																in_leadingPoint, 
																runMode, 
																&weldedLines);
		if (nextCleaveSequenceFinder.wasSequenceFound() == true)
		{
			FoundCleaveSequence discoveredSequence = nextCleaveSequenceFinder.getSelectedCleaveSequenceMeta();
			std::cout << ":::::::: Leading point is: " << in_leadingPoint.x << ", " << in_leadingPoint.y << ", " << in_leadingPoint.z << std::endl;
			std::cout << "||||||||| neighboring cleave sequence was found, stats are: " << std::endl;
			std::cout << "| cleave sequence ID: " << discoveredSequence.cleaveSequenceID << std::endl;
			std::cout << "| distance:           " << discoveredSequence.distance << std::endl;
			std::cout << "| point:              " << discoveredSequence.cleaveSequenceTracingBeginPoint.x << ", " << discoveredSequence.cleaveSequenceTracingBeginPoint.y << ", " << discoveredSequence.cleaveSequenceTracingBeginPoint.z << std::endl;

			CleaveSequenceMeta* fetchedCleaveSequenceMeta = metaTracker.fetchCleaveSequence(discoveredSequence.cleaveSequenceID);
			currentHierarchyPositionOfLatestCleaveSequence = fetchedCleaveSequenceMeta->cleaveSequencePtr->hierarchyPosition;			// update the current hierarchy position to be the value of the neighboring CleaveSequence we just found.
			fetchedCleaveSequenceMeta->determineCrawlDirectionFromPoint(discoveredSequence.cleaveSequenceTracingBeginPoint);			// for the cleave sequence we found, determine the the direction we'll go in.
			int numberOfLinesToCrawl = fetchedCleaveSequenceMeta->numberOfRemainingLines;

			
			if (runMode == LineWelderRunMode::ENDING)
			{
				std::cout << "Performing special check for ending mode... " << std::endl;
				// before continuing with the next cleave sequence, check to see if there is a distance between the leadingPoint and cleaveSequenceTracingBeginPoint
				if (discoveredSequence.distance != 0.0f)
				{
					std::cout << "!!! Note, distance is NOT 0, need to insert a line. " << std::endl;
					insertNewWeldingLine(in_leadingPoint, discoveredSequence.cleaveSequenceTracingBeginPoint, sPolyRef->borderLines[currentBorderLineID].planarVector);
					//WeldedLine newLine(sequenceFinderStartPoint, selectedSequence.cleaveSequenceTracingBeginPoint, borderLineRef->planarVector);


					//weldedLinePoolRef->insertLineIntoPool(newLine);
				}
			}
			

			for (int x = 0; x < numberOfLinesToCrawl; x++)
			{
				std::cout << "| Crawling line... " << std::endl;
				CategorizedLine currentCategorizedLine = fetchedCleaveSequenceMeta->fetchNextCategorizedLineInSequence();
				insertNewWeldingLine(currentCategorizedLine.line.pointA, currentCategorizedLine.line.pointB, currentCategorizedLine.emptyNormal);

				std::cout << "| Current line, point A: " << currentCategorizedLine.line.pointA.x << ", " << currentCategorizedLine.line.pointA.y << ", " << currentCategorizedLine.line.pointA.z << std::endl;
				std::cout << "| Current line, point B: " << currentCategorizedLine.line.pointB.x << ", " << currentCategorizedLine.line.pointB.y << ", " << currentCategorizedLine.line.pointB.z << std::endl;

				currentLeadingPoint = currentCategorizedLine.line.fetchNextPointBasedOnCyclingDirection(foundDirection);
				//std::cout << "| Current leading point, as a result of crawling, is: " << currentLeadingPoint.x << ", " << currentLeadingPoint.y << ", " << currentLeadingPoint.z << std::endl;

				// for the last iteration in this loop, get the next border line ID from the final categorized line in the sequence,
				// so the welder knows where to start in the next iteration.
				if (x == (numberOfLinesToCrawl - 1))
				{
					nextBorderLineID = fetchedCleaveSequenceMeta->acquireNextBorderLineID();
					//std::cout << "##!!!!! Next border line ID will be: " << nextBorderLineID << std::endl;
				}
			}

			// check if the CleaveSequence has run all it's lines; if it has, we must remove it from the appropriate CleaveSequenceCandidateList.
			bool isCleaveSequenceComplete = fetchedCleaveSequenceMeta->checkIfCleaveSequenceHasRunallCategorizedLines();
			if (isCleaveSequenceComplete == true)
			{
				std::cout << "######### CleaveSequence is COMPLETE, removing from candidateList, the ID of: " << discoveredSequence.cleaveSequenceID << std::endl;
				candidateListMap.removeCandidateFromAllCandidateLists(discoveredSequence.cleaveSequenceID);
			}


		}
		else if (nextCleaveSequenceFinder.wasSequenceFound() == false)
		{
			std::cout << "!!! No neighbors found, from the leading point: " << currentLeadingPoint.x << ", " << currentLeadingPoint.y << ", " << currentLeadingPoint.z << std::endl;
			nextBorderLineID = sPolyRef->getNextBorderLineID(currentBorderLineID, foundDirection);
			updateLeadingPointAndInsertNewWeldingLineFromBorderLineData();

			//std::cout << "Next border LINE id will be: " << nextBorderLineID << std::endl;
			//std::cout << "Leading point is now: " << currentLeadingPoint.x << ", " << currentLeadingPoint.y << ", " << currentLeadingPoint.z << std::endl;
		}
		currentBorderLineID = nextBorderLineID;
	}
	else if (intersectRecorderRef->records.size() == 1)		// only do this if there is ONE CleaveSequence in the current border line.
	{
		//std::cout << "::: Note: border Line with ID: " << currentBorderLineID << " has only 1 intercept record. " << std::endl;
		NextCleaveSequenceFinder nextCleaveSequenceFinder(in_currentBorderLineID, 
																&sPolyRef->borderLines[in_currentBorderLineID], 
																&sPolyRef->cleaveMap, 
																foundDirection, 
																in_cleaveSequenceCandidateListRef, 
																in_finderStartingCleaveSequenceID,
																&metaTracker, 
																in_leadingPoint, 
																runMode, 
																&weldedLines);
		if (nextCleaveSequenceFinder.wasSequenceFound() == true)
		{
			FoundCleaveSequence discoveredSequence = nextCleaveSequenceFinder.getSelectedCleaveSequenceMeta();

			std::cout << ":::::::: Leading point is: " << in_leadingPoint.x << ", " << in_leadingPoint.y << ", " << in_leadingPoint.z << std::endl;
			std::cout << "||||||||| neighboring cleave sequence was found, stats are: " << std::endl;
			std::cout << "| cleave sequence ID: " << discoveredSequence.cleaveSequenceID << std::endl;
			std::cout << "| distance:           " << discoveredSequence.distance << std::endl;
			std::cout << "| point:              " << discoveredSequence.cleaveSequenceTracingBeginPoint.x << ", " << discoveredSequence.cleaveSequenceTracingBeginPoint.y << ", " << discoveredSequence.cleaveSequenceTracingBeginPoint.z << std::endl;

			CleaveSequenceMeta* fetchedCleaveSequenceMeta = metaTracker.fetchCleaveSequence(discoveredSequence.cleaveSequenceID);
			currentHierarchyPositionOfLatestCleaveSequence = fetchedCleaveSequenceMeta->cleaveSequencePtr->hierarchyPosition;			// update the current hierarchy position to be the value of the neighboring CleaveSequence we just found.
			fetchedCleaveSequenceMeta->determineCrawlDirectionFromPoint(discoveredSequence.cleaveSequenceTracingBeginPoint);			// for the cleave sequence we found, determine the the direction we'll go in.
			int numberOfLinesToCrawl = fetchedCleaveSequenceMeta->numberOfRemainingLines;

			//std::cout << "!!! Number of lines to crawl: " << numberOfLinesToCrawl << std::endl;

			for (int x = 0; x < numberOfLinesToCrawl; x++)
			{
				//std::cout << "| Crawling line... " << std::endl;
				CategorizedLine currentCategorizedLine = fetchedCleaveSequenceMeta->fetchNextCategorizedLineInSequence();
				insertNewWeldingLine(currentCategorizedLine.line.pointA, currentCategorizedLine.line.pointB, currentCategorizedLine.emptyNormal);

				std::cout << ":::: Next categorized line point A: " << currentCategorizedLine.line.pointA.x << ", " << currentCategorizedLine.line.pointA.y << ", " << currentCategorizedLine.line.pointA.z << std::endl;
				std::cout << ":::: Next categorized line point B: " << currentCategorizedLine.line.pointB.x << ", " << currentCategorizedLine.line.pointB.y << ", " << currentCategorizedLine.line.pointB.z << std::endl;


				currentLeadingPoint = currentCategorizedLine.line.fetchNextPointBasedOnCyclingDirection(foundDirection);
				std::cout << "| Current leading point, as a result of crawling, is: " << currentLeadingPoint.x << ", " << currentLeadingPoint.y << ", " << currentLeadingPoint.z << std::endl;

				// for the last iteration in this loop, get the next border line ID from the final categorized line in the sequence,
				// so the welder knows where to start in the next iteration.
				if (x == (numberOfLinesToCrawl - 1))
				{
					nextBorderLineID = fetchedCleaveSequenceMeta->acquireNextBorderLineID();
					//std::cout << "##!!!!! Next border line ID will be: " << nextBorderLineID << std::endl;
				}
			}

			// check if the CleaveSequence has run all it's lines; if it has, we must remove it from the appropriate CleaveSequenceCandidateList.
			bool isCleaveSequenceComplete = fetchedCleaveSequenceMeta->checkIfCleaveSequenceHasRunallCategorizedLines();
			if (isCleaveSequenceComplete == true)
			{
				std::cout << "######### CleaveSequence is COMPLETE, removing from candidateList, the ID of: " << discoveredSequence.cleaveSequenceID << std::endl;
				candidateListMap.removeCandidateFromAllCandidateLists(discoveredSequence.cleaveSequenceID);
			}
			currentBorderLineID = nextBorderLineID;
		}
		else if (nextCleaveSequenceFinder.wasSequenceFound() == false)
		{
			std::cout << "!!! The single intercept record was already consumed!!! " << currentLeadingPoint.x << ", " << currentLeadingPoint.y << ", " << currentLeadingPoint.z << std::endl;
			nextBorderLineID = sPolyRef->getNextBorderLineID(currentBorderLineID, foundDirection);
			updateLeadingPointAndInsertNewWeldingLineFromBorderLineData();

			//std::cout << "Next border LINE id will be: " << nextBorderLineID << std::endl;
			//std::cout << "Leading point is now: " << currentLeadingPoint.x << ", " << currentLeadingPoint.y << ", " << currentLeadingPoint.z << std::endl;
		}
		currentBorderLineID = nextBorderLineID;
	}




	else if (intersectRecorderRef->records.size() == 0)		// only do this if there are ZERO CleaveSequences in the current border line.
	{
		int nextBorderLineID = sPolyRef->getNextBorderLineID(currentBorderLineID, foundDirection);
		//std::cout << ":: This border line has no records; producing line and proceeding to next border line with ID: " << nextBorderLineID << std::endl;

		updateLeadingPointAndInsertNewWeldingLineFromBorderLineData();

		//std::cout << "Next border LINE id will be: " << nextBorderLineID << std::endl;
		//std::cout << "Leading point is now: " << currentLeadingPoint.x << ", " << currentLeadingPoint.y << ", " << currentLeadingPoint.z << std::endl;

		currentBorderLineID = nextBorderLineID;

	}
}

void LineWelder::findRemainingWeldingLinesClosedCircuit(int in_currentBorderLineID, glm::vec3 in_leadingPoint, CleaveSequenceCandidateList* in_cleaveSequenceCandidateListRef, int in_finderStartingCleaveSequenceID)
{
	std::cout << "====> ## SPECIAL CASE ## -> Current border line ID is: " << in_currentBorderLineID << std::endl;
	std::cout << "====> ## SPECIAL CASE ## -> Current border line planar vector is: " << sPolyRef->borderLines[in_currentBorderLineID].planarVector.x << ", " << sPolyRef->borderLines[in_currentBorderLineID].planarVector.y << ", " << sPolyRef->borderLines[in_currentBorderLineID].planarVector.z << std::endl;
	
	CleaveSequenceMeta* fetchedCleaveSequenceMeta = metaTracker.fetchCleaveSequence(in_finderStartingCleaveSequenceID);
	glm::vec3 cleaveSequenceStartPoint = fetchedCleaveSequenceMeta->cleaveSequencePtr->cleavingLines.begin()->second.line.pointA;
	
	std::cout << "----> Cleave sequence start point is: " << cleaveSequenceStartPoint.x << ", " << cleaveSequenceStartPoint.y << ", " << cleaveSequenceStartPoint.z << std::endl;
	std::cout << "----> leading point is: " << in_leadingPoint.x << ", " << in_leadingPoint.y << ", " << in_leadingPoint.z << std::endl;

	fetchedCleaveSequenceMeta->determineCrawlDirectionFromPoint(cleaveSequenceStartPoint);			// for the cleave sequence we found, determine the the direction we'll go in.
	if (cleaveSequenceStartPoint != in_leadingPoint)
	{
		std::cout << "!!! Points are not equal, so let's insert a line." << std::endl;
		insertNewWeldingLine(in_leadingPoint, cleaveSequenceStartPoint, sPolyRef->borderLines[currentBorderLineID].planarVector);

		//fetchedCleaveSequenceMeta->determineCrawlDirectionFromPoint(cleaveSequenceStartPoint);			// for the cleave sequence we found, determine the the direction we'll go in.
		int numberOfLinesToCrawl = fetchedCleaveSequenceMeta->numberOfRemainingLines;
		for (int x = 0; x < numberOfLinesToCrawl; x++)
		{
			std::cout << "| Crawling line... " << std::endl;
			CategorizedLine currentCategorizedLine = fetchedCleaveSequenceMeta->fetchNextCategorizedLineInSequence();
			insertNewWeldingLine(currentCategorizedLine.line.pointA, currentCategorizedLine.line.pointB, currentCategorizedLine.emptyNormal);

			std::cout << "| Current line, point A: " << currentCategorizedLine.line.pointA.x << ", " << currentCategorizedLine.line.pointA.y << ", " << currentCategorizedLine.line.pointA.z << std::endl;
			std::cout << "| Current line, point B: " << currentCategorizedLine.line.pointB.x << ", " << currentCategorizedLine.line.pointB.y << ", " << currentCategorizedLine.line.pointB.z << std::endl;

			currentLeadingPoint = currentCategorizedLine.line.fetchNextPointBasedOnCyclingDirection(foundDirection);
		}

		bool isCleaveSequenceComplete = fetchedCleaveSequenceMeta->checkIfCleaveSequenceHasRunallCategorizedLines();
		if (isCleaveSequenceComplete == true)
		{
			std::cout << "######### CleaveSequence is COMPLETE, removing from candidateList, the ID of: " << in_finderStartingCleaveSequenceID << std::endl;
			candidateListMap.removeCandidateFromAllCandidateLists(in_finderStartingCleaveSequenceID);
		}
	}
}