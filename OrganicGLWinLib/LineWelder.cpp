#include "stdafx.h"
#include "LineWelder.h"

WeldedLinePool LineWelder::retrieveLinePool()
{
	return weldedLines;
}

void LineWelder::insertNewWeldingLine(glm::vec3 in_pointA, glm::vec3 in_pointB, glm::vec3 in_emptyNormal, bool in_wasLineInsertedAslice)
{
	WeldedLine beginningWeldedLine(in_pointA, in_pointB, in_emptyNormal);
	weldedLines.insertLineIntoPool(beginningWeldedLine);
	wasLastWeldingLineInsertedASlice = in_wasLineInsertedAslice;
}

void LineWelder::updateLeadingPointAndInsertNewWeldingLineFromBorderLineData()
{
	glm::vec3 newLineBeginPoint = currentLeadingPoint;
	glm::vec3 newLineEmptyNormal = sPolyRef->borderLines[currentBorderLineID].planarVector;
	currentLeadingPoint = sPolyRef->getBorderLineEndpoint(currentBorderLineID, foundDirection);
	insertNewWeldingLine(newLineBeginPoint, currentLeadingPoint, newLineEmptyNormal, false);
}

int LineWelder::getRemainingCandidateCount()
{
	return candidateListMap.remainingCandidateCount;
}

void LineWelder::clearLinePool()
{
	weldedLines.clearPool();
}

void LineWelder::printConsumedSequenceGroups()
{
	consumedSequenceGroups.printGroups();
}

void LineWelder::purgeNegligibleLinesInPool()
{
	weldedLines.purgeNegligibleLines();
}

bool LineWelder::getFaultValue()
{
	return wasFaultDetected;
}

void LineWelder::startWelding()
{
	//std::cout << "####### TESTING:######### cleave map size is: " << sPolyRef->cleaveMap.size() << std::endl;
	runMode = LineWelderRunMode::CONTINUE; // reset the value to CONTINUE.
	permit.resetPermit();				   // must be reset before continuing.
	// Step 1: get the first line, in the first cleave sequence, and determine how many border lines it has;
	// in addition, set that beginning cleave sequence to be the SUPER.

	int nextAvailableCandidate = candidateListMap.fetchNextAvailableCandidate();
	auto cleaveBegin = sPolyRef->cleaveMap.find(nextAvailableCandidate);

	beginningSequenceID = cleaveBegin->first;							

	auto startingCategorizedLine = cleaveBegin->second.cleavingLines.rbegin();
	auto endingCategorizedLine = cleaveBegin->second.cleavingLines.begin();

	/*
	auto borderLinesBegin = sPolyRef->borderLines.begin();
	auto borderLinesEnd = sPolyRef->borderLines.end();
	for (; borderLinesBegin != borderLinesEnd; borderLinesBegin++)
	{
		std::cout << "~~~~~~~ Border Line at index " << borderLinesBegin->first << " has " << borderLinesBegin->second.intersectRecorder.records.size() << " records. " << std::endl;
	}
	std::cout << "Finished printing intersectRecorder sizes. " << std::endl;
	int finishedVal = 3;
	std::cin >> finishedVal;
	*/

	//std::cout << ":::::::: Values of rbegin categorized line are: " << std::endl;
	//std::cout << "Point A: " << startingCategorizedLine->second.line.pointA.x << ", " << startingCategorizedLine->second.line.pointA.y << ", " << startingCategorizedLine->second.line.pointA.z << std::endl;
	//std::cout << "Point B: " << startingCategorizedLine->second.line.pointB.x << ", " << startingCategorizedLine->second.line.pointB.y << ", " << startingCategorizedLine->second.line.pointB.z << std::endl;
	//std::cout << "Empty normal: " << startingCategorizedLine->second.emptyNormal.x << ", " << startingCategorizedLine->second.emptyNormal.y << ", " << startingCategorizedLine->second.emptyNormal.z << std::endl;

	//std::cout << ":::::::: Values of begin categorized line are: " << std::endl;
	//std::cout << "Point A: " << endingCategorizedLine->second.line.pointA.x << ", " << endingCategorizedLine->second.line.pointA.y << ", " << endingCategorizedLine->second.line.pointA.z << std::endl;
	//std::cout << "Point B: " << endingCategorizedLine->second.line.pointB.x << ", " << endingCategorizedLine->second.line.pointB.y << ", " << endingCategorizedLine->second.line.pointB.z << std::endl;
	//std::cout << "Empty normal: " << endingCategorizedLine->second.emptyNormal.x << ", " << endingCategorizedLine->second.emptyNormal.y << ", " << endingCategorizedLine->second.emptyNormal.z << std::endl;

	lineWelderLogger.log("(LineWelder) :::::::: Cleave Map size is: ", sPolyRef->cleaveMap.size(), "\n");
	if (lineWelderLogger.isLoggingSet())
	{
		sPolyRef->printBorderLines();
	}

	lineWelderLogger.log("(LineWelder) :::::::: Number of lines in current cleave: ", cleaveBegin->second.cleavingLines.size(), "\n");

	lineWelderLogger.log("(LineWelder) :::::::: Values of rbegin categorized line are: ", "\n");
	lineWelderLogger.log("(LineWelder) Point A: ", startingCategorizedLine->second.line.pointA.x, ", ", startingCategorizedLine->second.line.pointA.y, ", ", startingCategorizedLine->second.line.pointA.z, "\n");
	lineWelderLogger.log("(LineWelder) Point B: ", startingCategorizedLine->second.line.pointB.x, ", ", startingCategorizedLine->second.line.pointB.y, ", ", startingCategorizedLine->second.line.pointB.z, "\n");
	lineWelderLogger.log("(LineWelder) Empty normal: ", startingCategorizedLine->second.emptyNormal.x, ", ", startingCategorizedLine->second.emptyNormal.y, ", ", startingCategorizedLine->second.emptyNormal.z, "\n");

	lineWelderLogger.log("(LineWelder) :::::::: Values of begin categorized line are: ", "\n");
	lineWelderLogger.log("(LineWelder) Point A: ", endingCategorizedLine->second.line.pointA.x, ", ", endingCategorizedLine->second.line.pointA.y, ", ", endingCategorizedLine->second.line.pointA.z, "\n");
	lineWelderLogger.log("(LineWelder) Point B: ", endingCategorizedLine->second.line.pointB.x, ", ", endingCategorizedLine->second.line.pointB.y, ", ", endingCategorizedLine->second.line.pointB.z, "\n");
	lineWelderLogger.log("(LineWelder) Empty normal: ", endingCategorizedLine->second.emptyNormal.x, ", ", endingCategorizedLine->second.emptyNormal.y, ", ", endingCategorizedLine->second.emptyNormal.z, "\n");

	bool slicedFlag = false;

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
	foundDirection = startingCategorizedLine->second.direction;			// remember, if the CategorizedLine's type is A_SLICE, the direction comes from pointA.
	if (foundDirection == CyclingDirection::NOVAL)	// needs to be handled, in case it does happen.
	{
		std::cout << "(LineWelder): warning, no direction found; run should terminate IMMEDIATELY! (setting wasFaultDetected to true)" << std::endl;
		wasFaultDetected = true;
	}
	BorderLinePointPair pointPair;		// point A of this pair is where the welding will begin from; point B of this pair is the beginning point of the WeldedLine, and point A is the end point of the WeldedLine.

	OperableIntSet unusableSet;
	if (startingCategorizedLine->second.line.numberOfBorderLines == 1)		// if there are multiple CategorizedLines in the cleave sequence, the first line in the sequence
																	// will have exactly one border line. The end of the sequence will have another border line.
	{
		//std::cout << "::: Branch hit for one border line... " << std::endl;
		lineWelderLogger.log("(LineWelder) ::: Branch hit for one border line... ", "\n");

		//pointPair = startingCategorizedLine->second.line.getBorderLinePointPair();
		currentLeadingPoint = startingCategorizedLine->second.line.getBorderPointFromSingularBorderLineCount();
		currentBorderLineID = startingCategorizedLine->second.line.getBorderLineIDFromSingularBorderLineCount();		
		endingBorderLineID = endingCategorizedLine->second.line.getBorderLineIDFromSingularBorderLineCount();
		CleaveSequenceMeta* beginningCleaveSequenceMeta = metaTracker.fetchCleaveSequence(beginningSequenceID);	
		currentCategorizedLine = beginningCleaveSequenceMeta->fetchFirstCategorizedLineForWelder();

		//std::cout << "::: First categorized line Aoint A: " << currentCategorizedLine.line.pointA.x << ", " << currentCategorizedLine.line.pointA.y << ", " << currentCategorizedLine.line.pointA.z
			//												<< " | Point B: " << currentCategorizedLine.line.pointB.x << ", " << currentCategorizedLine.line.pointB.y << ", " << currentCategorizedLine.line.pointB.z << std::endl;
		//std::cout << "::: First categorized line empty normal: " << currentCategorizedLine.emptyNormal.x << ", " << currentCategorizedLine.emptyNormal.y << ", " << currentCategorizedLine.emptyNormal.z << std::endl;
		//std::cout << "(Border Lines = 1) currentBorderLineID: " << currentBorderLineID << std::endl;
		//std::cout << "(Border Lines = 1) endingBorderLineID: " << endingBorderLineID << std::endl;

		// determine unusable cleave sequences for this call to startWelding(); see commit from 3/8/2021
		UsableCleaveSequenceCalculator calculator(beginningSequenceID,
													currentBorderLineID,
													currentCategorizedLine,
													currentLeadingPoint,
													&candidateListMap.candidateMap[currentBorderLineID],
													&sPolyRef->cleaveMap,
													lineWelderLoggerDebugLevel);
		unusableSet = calculator.getUnusables();
	}
	else if (startingCategorizedLine->second.line.numberOfBorderLines == 2)	// if there is just one line, it'll have two border lines; now we must
																			// get the borders from the pointA and pointB of this line; 
																			// would be used for Lines of type:
																			// -A_SLICE
																			// -A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE
	{
		slicedFlag = true;

		//std::cout << "::: Branch hit for two border lines... " << std::endl;
		lineWelderLogger.log("::: Branch hit for two border lines... ", "\n");


		if (lineWelderLogger.isLoggingSet())
		{

			lineWelderLogger.log("(LineWelder) (---- numberOfBorderLines = 2, before logic) printing Candidiate lists, PRIOR TO removal of this beginningSequenceID: ", beginningSequenceID, "\n");
			candidateListMap.printCandidateLists();
		}

		BorderLineIDPair retrievedPair = startingCategorizedLine->second.line.getBorderLineIDPair();
		pointPair = startingCategorizedLine->second.line.getBorderLinePointPair();
		currentLeadingPoint = pointPair.pointB;			
		currentBorderLineID = retrievedPair.pointBBorderLineID;											
		endingBorderLineID = retrievedPair.pointABorderLineID;
		CleaveSequenceMeta* beginningCleaveSequenceMeta = metaTracker.fetchCleaveSequence(beginningSequenceID);		
		currentCategorizedLine = beginningCleaveSequenceMeta->fetchFirstCategorizedLineForWelder();

		// determine unusable cleave sequences for this call to startWelding(); see commit from 3/8/2021
		UsableCleaveSequenceCalculator calculator(beginningSequenceID, 
													currentBorderLineID, 
													currentCategorizedLine, 
													currentLeadingPoint, 
													&candidateListMap.candidateMap[currentBorderLineID], 
													&sPolyRef->cleaveMap,
													lineWelderLoggerDebugLevel);
		unusableSet = calculator.getUnusables();

		if (lineWelderLogger.isLoggingSet())
		{

			lineWelderLogger.log("(LineWelder) (---- numberOfBorderLines = 2, after logic) printing Candidiate lists, PRIOR TO removal of this beginningSequenceID: ", beginningSequenceID, "\n");
			candidateListMap.printCandidateLists();
		}

		//std::cout << "::: First categorized line, Point A: " << currentCategorizedLine.line.pointA.x << ", " << currentCategorizedLine.line.pointA.y << ", " << currentCategorizedLine.line.pointA.z
			//<< " | Point B: " << currentCategorizedLine.line.pointB.x << ", " << currentCategorizedLine.line.pointB.y << ", " << currentCategorizedLine.line.pointB.z << std::endl;


		//std::cout << "::: First categorized line empty normal: " << currentCategorizedLine.emptyNormal.x << ", " << currentCategorizedLine.emptyNormal.y << ", " << currentCategorizedLine.emptyNormal.z << std::endl;
		//std::cout << "currentBorderLineID: " << currentBorderLineID << std::endl;
		//std::cout << "endingBorderLineID: " << endingBorderLineID << std::endl;

		//int someVal = 3;
		//std::cin >> someVal;

		lineWelderLogger.log("(LineWelder) ::: First categorized line, Point A: ", currentCategorizedLine.line.pointA.x, ", ", currentCategorizedLine.line.pointA.y, ", ", currentCategorizedLine.line.pointA.z,
			" | Point B: ", currentCategorizedLine.line.pointB.x, ", ", currentCategorizedLine.line.pointB.y, ", ", currentCategorizedLine.line.pointB.z, "\n");
		lineWelderLogger.log("(LineWelder) ::: First categorized line empty normal : ", currentCategorizedLine.emptyNormal.x, ", ", currentCategorizedLine.emptyNormal.y, ", ", currentCategorizedLine.emptyNormal.z, "\n");
		lineWelderLogger.log("(LineWelder) currentBorderLineID: ", currentBorderLineID, "\n");
		lineWelderLogger.log("(LineWelder) endingBorderLineID: ", endingBorderLineID, "\n");
		lineWelderLogger.log("(LineWelder) enter number to continue...", "\n");
		lineWelderLogger.waitForDebugInput();



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
		permit.setPermitInitialData(PermitType::TYPICAL, beginningSequenceID);

		// if we are not in a A_SLICE, do this
		// if { size of cleave sequence != 1 AND != A_SLICE }
		// { 
		//int loopCount = 0;
		while (currentBorderLineID != endingBorderLineID)
		{
			// pass in: the border line ID, the ID of the CleaveSequence in the CleaveMap, 
			// the ID of the CategorizedLine in the CleaveSequence, the direction

			//std::cout << "! currentBorderLineID: " << currentBorderLineID << std::endl;
			//std::cout << "! endingBorderLineID: " << endingBorderLineID << std::endl;

			//std::cout << "####  Begin and end border line differ..." << std::endl;
			//if
			//(
				//(loopCount == 0)
				//&&
				//(slicedFlag == true)
			//)
			//{
				//wasLastWeldingLineInsertedASlice = true;
			//}

			if (lineWelderLogger.isLoggingSet())
			{

				lineWelderLogger.log("(LineWelder) printing Candidiate lists, PRIOR TO removal of this beginningSequenceID: ", beginningSequenceID, "\n");
				candidateListMap.printCandidateLists();
			}


			findRemainingWeldingLines(currentBorderLineID, currentLeadingPoint, &candidateListMap.candidateMap[currentBorderLineID], cleaveBegin->first, unusableSet);

			if (lineWelderLogger.isLoggingSet())
			{

				lineWelderLogger.log("(LineWelder) printing Candidiate lists, after removal of this beginningSequenceID: ", beginningSequenceID, "\n");
				candidateListMap.printCandidateLists();
			}

			//std::cout << "##!!! Iterating while... " << std::endl;
			//int someVal = 5;
			//std::cin >> someVal;
			//loopCount++;
		}

		if (slicedFlag == true)
		{
			//insertNewWeldingLine(in_leadingPoint, discoveredSequence.cleaveSequenceTracingBeginPoint, sPolyRef->borderLines[currentBorderLineID].planarVector);
			lineWelderLogger.log("(LineWelder) slicedFlag found as true...; will insert this line and remove from candidate list.", "\n");
			if (currentLeadingPoint != currentCategorizedLine.line.pointA)
			{
				insertNewWeldingLine(currentLeadingPoint, currentCategorizedLine.line.pointA, sPolyRef->borderLines[currentBorderLineID].planarVector, true);
			}


	

			candidateListMap.removeCandidateFromAllCandidateLists(beginningSequenceID);
			consumedSequenceGroups.insertConsumedCleaveSequence(runIteration, beginningSequenceID);	// make sure to keep track of all consumed sequences.

		}

		// }
		// else
		// {
		// ...
		// }


		//	Run in ENDING mode when we are on endingBorderLineID, and continue to find remaining lines.
		//std::cout << "::> Number of remaining lines in the beginning cleave sequence is: " << beginningCleaveSequenceMeta->numberOfRemainingLines << std::endl;
		
	}

	else if (currentBorderLineID == endingBorderLineID)		// the cleave sequence begins and ends on the same border line
	{
		runMode = LineWelderRunMode::ENDING;		// this may not need to be set here; be sure to experiment with this later. (11/16/2020)
		permit.setPermitInitialData(PermitType::SAME_BORDER_LINE, beginningSequenceID);
		//std::cout << "############# SPECIAL LOGIC NEEDED. " << std::endl;
		findRemainingWeldingLines(currentBorderLineID, currentLeadingPoint, &candidateListMap.candidateMap[currentBorderLineID], cleaveBegin->first, unusableSet);
	}


	runMode = LineWelderRunMode::ENDING;
	while (beginningCleaveSequenceMeta->numberOfRemainingLines > 0)
	{
		findRemainingWeldingLines(currentBorderLineID, currentLeadingPoint, &candidateListMap.candidateMap[currentBorderLineID], cleaveBegin->first, unusableSet);
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
	
	if (lineWelderLogger.isLoggingSet() == true)
	{
		lineWelderLogger.log("(LineWelder) >>>> starting printing out candidate lists in candidateListMap.", "\n");
		candidateListMap.printCandidateLists();
		lineWelderLogger.log("(LineWelder) >>>> finished printing out candidate lists in candidateListMap.", "\n");
	}

	candidateListMap.updateRemainingCandidateCount();		// when the LineWelder has completed the run, update the line counts in each CleaveSequenceCandidateList in the candidateListMap, 
															// (so that we know when to finish, once we get to a count of 0.)
															

	//std::cout << "### ............................................................................>>>>>>>>>>>>>> Welding complete; lines are ready for WeldedTriangleGroupBuilder. Enter number to continue. " << std::endl;
	//int someVal = 3;
	//std::cin >> someVal;

	lineWelderLogger.log("(LineWelder) ### ............................................................................>>>>>>>>>>>>>> Welding complete; lines are ready for WeldedTriangleGroupBuilder. Enter number to continue. ", "\n");
	lineWelderLogger.waitForDebugInput();
	// use the unused point of the categorized line to determine how to quat to Z = 0 (Z-planar).

	runIteration++;		// the index to use for inserting into consumedSequenceGroups must be incremented every time this function is called, but only at the end.
}

void LineWelder::getCleaveSequenceCandidateListMap()
{
	candidateListMap = sPolyRef->buildCleaveSequenceCandidateListMap();
	candidateListMap.updateRemainingCandidateCount();

	lineWelderLogger.log("(LineWelder) >>>> Building of candidateListMap complete; candidateListMap.remainingCandidateCount is: ", candidateListMap.remainingCandidateCount, "\n");
	lineWelderLogger.waitForDebugInput();
}

void LineWelder::getCleaveSequenceMetaTracker()
{
	metaTracker = sPolyRef->buildCleaveSequenceMetaTracker();
}

void LineWelder::findRemainingWeldingLines(int in_currentBorderLineID, 
											glm::vec3 in_leadingPoint, 
											CleaveSequenceCandidateList* in_cleaveSequenceCandidateListRef, 
											int in_finderStartingCleaveSequenceID,
											OperableIntSet in_unusableSet)
{
	//std::cout << "----------------------------------> Welding leading point for this line is: " << in_leadingPoint.x << ", " << in_leadingPoint.y << ", " << in_leadingPoint.z << std::endl;
	lineWelderLogger.log("(LineWelder) ----------------------------------> Welding leading point for this line is: ", in_leadingPoint.x, ", ", in_leadingPoint.y, ", ", in_leadingPoint.z, "\n");

	// find any neighboring cleave lines that exist on the same border line, if they exist. If they do exist, fetch the next
	// pass in these parameters:
	// -the border line that the finder needs to run on
	// -a reference to border line that the finder will run on
	// -a referenece to the SPoly's cleave map
	// -the CyclingDirection the LineWelder is running in
	// -a reference to a set indicating the candidates (that is, selectable CleaveSequences that haven't been consumed/used) that exist on the border line that the finder runs on
	// -the ID of the current CleaveSequence this finder will start from

	//std::cout << "====> Current border line ID is: " << in_currentBorderLineID << std::endl;
	//std::cout << "====> Current border line planar vector is: " << sPolyRef->borderLines[in_currentBorderLineID].planarVector.x << ", " << sPolyRef->borderLines[in_currentBorderLineID].planarVector.y << ", " << sPolyRef->borderLines[in_currentBorderLineID].planarVector.z << std::endl;

	//	// get a ref to the intersect recorder in our current border line.
	BorderLineIntersectRecorder* intersectRecorderRef = &sPolyRef->borderLines[in_currentBorderLineID].intersectRecorder;
	int nextBorderLineID = 0;	// will be set by conditions below...
	if (intersectRecorderRef->records.size() > 1)	// only do this check if there are MULTIPLE CleaveSequences in a BorderLine
	{
		//std::cout << "---> Entered branch for record size > 1)" << std::endl;
		NextCleaveSequenceFinder nextCleaveSequenceFinder(in_currentBorderLineID, 
																&sPolyRef->borderLines[in_currentBorderLineID], 
																&sPolyRef->cleaveMap, foundDirection, 
																in_cleaveSequenceCandidateListRef, 
																in_finderStartingCleaveSequenceID, 
																&metaTracker, 
																in_leadingPoint, 
																runMode, 
																&weldedLines,
																permit,
																lineWelderLoggerDebugLevel,
																in_unusableSet);
		if (nextCleaveSequenceFinder.wasSequenceFound() == true)
		{
			std::cout << "--> Next sequence was found! " << std::endl;
			FoundCleaveSequence discoveredSequence = nextCleaveSequenceFinder.getSelectedCleaveSequenceMeta();
			//std::cout << ":::::::: (Multiple Intersect Records) Leading point is: " << in_leadingPoint.x << ", " << in_leadingPoint.y << ", " << in_leadingPoint.z << std::endl;
			//std::cout << "||||||||| neighboring cleave sequence was found, stats are: " << std::endl;
			//std::cout << "| cleave sequence ID: " << discoveredSequence.cleaveSequenceID << std::endl;
			std::cout << "| distance:           " << discoveredSequence.distance << std::endl;
			std::cout << "| point:              " << discoveredSequence.cleaveSequenceTracingBeginPoint.x << ", " << discoveredSequence.cleaveSequenceTracingBeginPoint.y << ", " << discoveredSequence.cleaveSequenceTracingBeginPoint.z << std::endl;

			//std::cout << "(1) --> Next sequence was found! " << std::endl;

			CleaveSequenceMeta* fetchedCleaveSequenceMeta = metaTracker.fetchCleaveSequence(discoveredSequence.cleaveSequenceID);

			//std::cout << "(2) --> Next sequence was found! " << std::endl;

			currentHierarchyPositionOfLatestCleaveSequence = fetchedCleaveSequenceMeta->cleaveSequencePtr->hierarchyPosition;			// update the current hierarchy position to be the value of the neighboring CleaveSequence we just found.

			//std::cout << "(3) --> Next sequence was found! " << std::endl;

			fetchedCleaveSequenceMeta->determineCrawlDirectionFromPoint(discoveredSequence.cleaveSequenceTracingBeginPoint);			// for the cleave sequence we found, determine the the direction we'll go in.

			//std::cout << "(4) --> Next sequence was found! " << std::endl;

			int numberOfLinesToCrawl = fetchedCleaveSequenceMeta->numberOfRemainingLines;

			//std::cout << "!! Number of lines to crawl: " << numberOfLinesToCrawl << std::endl;

			
			// It is unknown why the below if statement was implemented; the distance check should always be done when transitioning
			// between the current CleaveSequence and the next cleave sequence that was found. (5/29/2021). This code will probably be deleted
			// upon further review.
			/*
			if 
			(
				(runMode == LineWelderRunMode::ENDING)
				||
				wasLastWeldingLineInsertedASlice == true
			)
			
			{
			*/
			
				//std::cout << "Performing special check for ending / last slice inserted mode... " << std::endl;
				lineWelderLogger.log("(LineWelder) Checking if line needs to be inserted between the two sequences...", "\n");
				// before continuing with the next cleave sequence, check to see if there is a distance between the leadingPoint and cleaveSequenceTracingBeginPoint
				if (discoveredSequence.distance != 0.0f)
				{
					//std::cout << "!!! Note, distance is NOT 0, need to insert a line. " << std::endl;
					insertNewWeldingLine(in_leadingPoint, discoveredSequence.cleaveSequenceTracingBeginPoint, sPolyRef->borderLines[currentBorderLineID].planarVector, false);
					//WeldedLine newLine(sequenceFinderStartPoint, selectedSequence.cleaveSequenceTracingBeginPoint, borderLineRef->planarVector);


					//weldedLinePoolRef->insertLineIntoPool(newLine);
				}
			//}
			

			for (int x = 0; x < numberOfLinesToCrawl; x++)
			{
				//std::cout << "| Crawling line... " << std::endl;
				lineWelderLogger.log("(LineWelder) | Crawling line...", "\n");
				CategorizedLine currentCategorizedLine = fetchedCleaveSequenceMeta->fetchNextCategorizedLineInSequence();	// fetch the next line, swapping the points of it if in REVERSE.
				insertNewWeldingLine(currentCategorizedLine.line.pointA, currentCategorizedLine.line.pointB, currentCategorizedLine.emptyNormal, false);

				//std::cout << "| Current line, point A: " << currentCategorizedLine.line.pointA.x << ", " << currentCategorizedLine.line.pointA.y << ", " << currentCategorizedLine.line.pointA.z << std::endl;
				//std::cout << "| Current line, point B: " << currentCategorizedLine.line.pointB.x << ", " << currentCategorizedLine.line.pointB.y << ", " << currentCategorizedLine.line.pointB.z << std::endl;

				currentLeadingPoint = currentCategorizedLine.line.fetchNextPointBasedOnCyclingDirection(foundDirection);	// fetch point B of the current line  
																															// (this function needs to be updated, since the foundDirection parameter is obsolete. 12/31/2020)
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
				//std::cout << "######### CleaveSequence is COMPLETE, removing from candidateList, the ID of: " << discoveredSequence.cleaveSequenceID << std::endl;
				candidateListMap.removeCandidateFromAllCandidateLists(discoveredSequence.cleaveSequenceID);
				consumedSequenceGroups.insertConsumedCleaveSequence(runIteration, discoveredSequence.cleaveSequenceID);	// make sure to keep track of all consumed sequences.
			}


		}
		else if (nextCleaveSequenceFinder.wasSequenceFound() == false)
		{
			//std::cout << "!!! No neighbors found, from the leading point: " << currentLeadingPoint.x << ", " << currentLeadingPoint.y << ", " << currentLeadingPoint.z << std::endl;
			nextBorderLineID = sPolyRef->getNextBorderLineID(currentBorderLineID, foundDirection);
			if (lineWelderLogger.isLoggingSet() == true)
			{
				lineWelderLogger.log("(LineWelder) found next border line ID; original ID was ", currentBorderLineID, "; new ID is ", nextBorderLineID, "; direction was ");
				if (foundDirection == CyclingDirection::FORWARD)
				{
					lineWelderLogger.log("FORWARD", "\n");
				}
				else if (foundDirection == CyclingDirection::REVERSE)
				{
					lineWelderLogger.log("REVERSE", "\n");
				}
			}
			updateLeadingPointAndInsertNewWeldingLineFromBorderLineData();

			//std::cout << "Next border LINE id will be: " << nextBorderLineID << std::endl;
			//std::cout << "Leading point is now: " << currentLeadingPoint.x << ", " << currentLeadingPoint.y << ", " << currentLeadingPoint.z << std::endl;
		}
		currentBorderLineID = nextBorderLineID;
	}
	else if (intersectRecorderRef->records.size() == 1)		// only do this if there is ONE CleaveSequence in the current border line.
	{
		//std::cout << "::: Note: border Line with ID: " << currentBorderLineID << " has only 1 intercept record. " << std::endl;
		//std::cout << "::: Value of in_currentBorderLineID: " << in_currentBorderLineID << std::endl;

		lineWelderLogger.log("(LineWelder) ::: Note: border Line with ID: ", currentBorderLineID, " has only 1 intercept record. ", "\n");
		lineWelderLogger.log("(LineWelder) ::: Value of in_currentBorderLineID : ", in_currentBorderLineID, "\n");

		NextCleaveSequenceFinder nextCleaveSequenceFinder(in_currentBorderLineID, 
																&sPolyRef->borderLines[in_currentBorderLineID], 
																&sPolyRef->cleaveMap, 
																foundDirection, 
																in_cleaveSequenceCandidateListRef, 
																in_finderStartingCleaveSequenceID,
																&metaTracker, 
																in_leadingPoint, 
																runMode, 
																&weldedLines, 
																permit,
																lineWelderLoggerDebugLevel,
																in_unusableSet);
		if (nextCleaveSequenceFinder.wasSequenceFound() == true)
		{
			FoundCleaveSequence discoveredSequence = nextCleaveSequenceFinder.getSelectedCleaveSequenceMeta();

			//std::cout << ":::::::: (Single Intersect Record) Leading point is: " << in_leadingPoint.x << ", " << in_leadingPoint.y << ", " << in_leadingPoint.z << std::endl;
			//std::cout << "||||||||| neighboring cleave sequence was found, stats are: " << std::endl;
			//std::cout << "| cleave sequence ID: " << discoveredSequence.cleaveSequenceID << std::endl;
			//std::cout << "| distance:           " << discoveredSequence.distance << std::endl;
			//std::cout << "| point:              " << discoveredSequence.cleaveSequenceTracingBeginPoint.x << ", " << discoveredSequence.cleaveSequenceTracingBeginPoint.y << ", " << discoveredSequence.cleaveSequenceTracingBeginPoint.z << std::endl;

			CleaveSequenceMeta* fetchedCleaveSequenceMeta = metaTracker.fetchCleaveSequence(discoveredSequence.cleaveSequenceID);
			currentHierarchyPositionOfLatestCleaveSequence = fetchedCleaveSequenceMeta->cleaveSequencePtr->hierarchyPosition;			// update the current hierarchy position to be the value of the neighboring CleaveSequence we just found.
			fetchedCleaveSequenceMeta->determineCrawlDirectionFromPoint(discoveredSequence.cleaveSequenceTracingBeginPoint);			// for the cleave sequence we found, determine the the direction we'll go in.
			int numberOfLinesToCrawl = fetchedCleaveSequenceMeta->numberOfRemainingLines;

			if (discoveredSequence.distance != 0.0f)
			{
				//std::cout << "!!! Note, distance is NOT 0, need to insert a line. " << std::endl;
				insertNewWeldingLine(in_leadingPoint, discoveredSequence.cleaveSequenceTracingBeginPoint, sPolyRef->borderLines[currentBorderLineID].planarVector, false);
				//WeldedLine newLine(sequenceFinderStartPoint, selectedSequence.cleaveSequenceTracingBeginPoint, borderLineRef->planarVector);


				//weldedLinePoolRef->insertLineIntoPool(newLine);
			}

			//std::cout << "!!! Number of lines to crawl: " << numberOfLinesToCrawl << std::endl;

			for (int x = 0; x < numberOfLinesToCrawl; x++)
			{
				//std::cout << "| Crawling line... " << std::endl;
				CategorizedLine currentCategorizedLine = fetchedCleaveSequenceMeta->fetchNextCategorizedLineInSequence();		// fetch the next line, swapping the points of it if in REVERSE.

				insertNewWeldingLine(currentCategorizedLine.line.pointA, currentCategorizedLine.line.pointB, currentCategorizedLine.emptyNormal, false);

				//std::cout << ":::: Next categorized line point A: " << currentCategorizedLine.line.pointA.x << ", " << currentCategorizedLine.line.pointA.y << ", " << currentCategorizedLine.line.pointA.z << std::endl;
				//std::cout << ":::: Next categorized line point B: " << currentCategorizedLine.line.pointB.x << ", " << currentCategorizedLine.line.pointB.y << ", " << currentCategorizedLine.line.pointB.z << std::endl;

					
				currentLeadingPoint = currentCategorizedLine.line.fetchNextPointBasedOnCyclingDirection(foundDirection);		// fetch point B of the current line  
																																// (this function needs to be updated, since the foundDirection parameter is obsolete. 12/31/2020)
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
				//std::cout << "######### CleaveSequence is COMPLETE, removing from candidateList, the ID of: " << discoveredSequence.cleaveSequenceID << std::endl;
				candidateListMap.removeCandidateFromAllCandidateLists(discoveredSequence.cleaveSequenceID);
				consumedSequenceGroups.insertConsumedCleaveSequence(runIteration, discoveredSequence.cleaveSequenceID);	// make sure to keep track of all consumed sequences.
			}
			currentBorderLineID = nextBorderLineID;
		}
		else if (nextCleaveSequenceFinder.wasSequenceFound() == false)
		{
			//std::cout << "!!! The single intercept record was already consumed!!! Will insert new line... " << currentLeadingPoint.x << ", " << currentLeadingPoint.y << ", " << currentLeadingPoint.z << std::endl;
			nextBorderLineID = sPolyRef->getNextBorderLineID(currentBorderLineID, foundDirection);
			if (lineWelderLogger.isLoggingSet() == true)
			{
				lineWelderLogger.log("(LineWelder) found next border line ID; original ID was ", currentBorderLineID, "; new ID is ", nextBorderLineID, "; direction was ");
				if (foundDirection == CyclingDirection::FORWARD)
				{
					lineWelderLogger.log("FORWARD", "\n");
				}
				else if (foundDirection == CyclingDirection::REVERSE)
				{
					lineWelderLogger.log("REVERSE", "\n");
				}
			}
			updateLeadingPointAndInsertNewWeldingLineFromBorderLineData();
			lineWelderLogger.log("(LineWelder): leading point value is now: ", currentLeadingPoint.x, ", ", currentLeadingPoint.y, ", ", currentLeadingPoint.z, "\n");

			//std::cout << "Next border LINE id will be: " << nextBorderLineID << std::endl;
			//std::cout << "Leading point is now: " << currentLeadingPoint.x << ", " << currentLeadingPoint.y << ", " << currentLeadingPoint.z << std::endl;
		}
		currentBorderLineID = nextBorderLineID;
	}




	else if (intersectRecorderRef->records.size() == 0)		// only do this if there are ZERO CleaveSequences in the current border line.
	{
		int nextBorderLineID = sPolyRef->getNextBorderLineID(currentBorderLineID, foundDirection);
		if (lineWelderLogger.isLoggingSet() == true)
		{
			lineWelderLogger.log("(LineWelder) found next border line ID; original ID was ", currentBorderLineID, "; new ID is ", nextBorderLineID, "; direction was ");
			if (foundDirection == CyclingDirection::FORWARD)
			{
				lineWelderLogger.log("FORWARD", "\n");
			}
			else if (foundDirection == CyclingDirection::REVERSE)
			{
				lineWelderLogger.log("REVERSE", "\n");
			}
		}
		//std::cout << ":: This border line has no records; producing line and proceeding to next border line with ID: " << nextBorderLineID << std::endl;

		updateLeadingPointAndInsertNewWeldingLineFromBorderLineData();

		//std::cout << "Next border LINE id will be: " << nextBorderLineID << std::endl;
		//std::cout << "Leading point is now: " << currentLeadingPoint.x << ", " << currentLeadingPoint.y << ", " << currentLeadingPoint.z << std::endl;

		currentBorderLineID = nextBorderLineID;

	}

	// update the permit action flag to false.
	permit.first_permit_action = false;

	//std::cout << "#########_> completed iteration of findRemainingWeldingLines" << std::endl;
	lineWelderLogger.log("(LineWelder) #########_> completed iteration of findRemainingWeldingLines", "\n");
	lineWelderLogger.log("(LineWelder) #################-> size of line pool, after this iteration: ", weldedLines.getPoolSize(), "\n");
	//int someVal = 3;
	//std::cin >> someVal;
}
