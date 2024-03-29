#include "stdafx.h"
#include "CleaveSequenceFactory.h"

void CleaveSequenceFactory::addCategorizedLine(CategorizedLine in_categorizedLine)
{
	bool duplicateFound = lineManager.checkForDuplicateCategorizedLine(in_categorizedLine);
	if (duplicateFound == false)
	{
		lineManager.insertLineAndReturnInsertedIndex(in_categorizedLine); // new test for 4/2/2021, for refactor.
	}
}

void CleaveSequenceFactory::copyCategorizedLinesFromLinePool(CategorizedLinePool* in_categorizedLinePoolRef)
{
	auto inPoolLinesBegin = in_categorizedLinePoolRef->linePool.begin();
	auto inPoolLinesEnd = in_categorizedLinePoolRef->linePool.end();
	for (; inPoolLinesBegin != inPoolLinesEnd; inPoolLinesBegin++)
	{
		addCategorizedLine(*inPoolLinesBegin);
	}
}

void CleaveSequenceFactory::insertNonboundLine(CategorizedLine in_line)
{
	lineManager.insertLineAndReturnInsertedIndex(in_line); // new test for 4/2/2021, for refactor.
}

CategorizedLine CleaveSequenceFactory::fetchAndRemoveNonboundWithGroupMapLocationPush(int in_fetchIndex, std::vector<CategorizedLineGroupLocation>* in_categorizedLineGroupLocationVectorRef)
{
	CategorizedLine fetchedLine = lineManager.fetchAndRemoveWithGroupMapLocationPush(IntersectionType::NON_BOUND, in_fetchIndex, in_categorizedLineGroupLocationVectorRef);
	return fetchedLine;
}

CategorizedLine CleaveSequenceFactory::fetchAndRemovePartialBoundWithGroupMapLocationPush(int in_fetchIndex, std::vector<CategorizedLineGroupLocation>* in_categorizedLineGroupLocationVectorRef)
{
	CategorizedLine fetchedLine = lineManager.fetchAndRemoveWithGroupMapLocationPush(IntersectionType::PARTIAL_BOUND, in_fetchIndex, in_categorizedLineGroupLocationVectorRef);
	return fetchedLine;
}

CategorizedLine CleaveSequenceFactory::fetchAndRemoveASliceWithGroupMapLocationPush(int in_fetchIndex, std::vector<CategorizedLineGroupLocation>* in_categorizedLineGroupLocationVectorRef)
{
	CategorizedLine fetchedLine = lineManager.fetchAndRemoveWithGroupMapLocationPush(IntersectionType::A_SLICE, in_fetchIndex, in_categorizedLineGroupLocationVectorRef);
	return fetchedLine;
}

CategorizedLine CleaveSequenceFactory::fetchAndRemoveASliceSingleInterceptsPointPreciseWithGroupMapLocationPush(int in_fetchIndex, std::vector<CategorizedLineGroupLocation>* in_categorizedLineGroupLocationVectorRef)
{
	CategorizedLine fetchedLine = lineManager.fetchAndRemoveWithGroupMapLocationPush(IntersectionType::A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE, in_fetchIndex, in_categorizedLineGroupLocationVectorRef);
	return fetchedLine;
}

CategorizedLine CleaveSequenceFactory::fetchAndRemoveInterceptPointPreciseWithGroupMapLocationPush(int in_fetchIndex, std::vector<CategorizedLineGroupLocation>* in_categorizedLineGroupLocationVectorRef)
{
	CategorizedLine fetchedLine = lineManager.fetchAndRemoveWithGroupMapLocationPush(IntersectionType::INTERCEPTS_POINT_PRECISE, in_fetchIndex, in_categorizedLineGroupLocationVectorRef);
	return fetchedLine;
}


bool CleaveSequenceFactory::doesFactoryContainLines()
{
	return lineManager.doAnyLinesExist();
}

void CleaveSequenceFactory::setFactoryDebugLevel(PolyDebugLevel in_polyDebugLevel)
{
	cleaveSequenceFactoryLogger.setDebugLevel(in_polyDebugLevel);
	lineManager.setCategorizedLineManagerDebugLevel(in_polyDebugLevel);
}

void CleaveSequenceFactory::setMergerDebugLevel(PolyDebugLevel in_polyDebugLevel)
{
	mergerDebugLevel = in_polyDebugLevel;
}

void CleaveSequenceFactory::setFactoryBoundaryOrientationOption(BoundaryOrientation in_optionalFactoryOrientation)
{
	optionalFactoryOrientation = in_optionalFactoryOrientation;
}

void CleaveSequenceFactory::loadCategorizedLineMapReferencesIntoQuatPointsExcludeEmptyNormals(QuatRotationPoints* in_quatRotationPointsRef)
{
	lineManager.loadCategorizedLinePoints(in_quatRotationPointsRef);
}

void CleaveSequenceFactory::loadCategorizedLineEmptyNormalsIntoQuatPoints(QuatRotationPoints* in_quatRotationPointsRef)
{
	lineManager.loadCategorizedLineEmptyNormals(in_quatRotationPointsRef);
}



InvalidCleaveSequences CleaveSequenceFactory::constructAndExportCleaveSequences(std::map<int, CleaveSequence>* in_cleaveMapRef, 
																				std::map<int, SPolyBorderLines> in_borderLineArrayRef, 
																				MassManipulationMode in_massManipulationMode, 
																				CleaveSequenceMergeMode in_cleaveSequenceMergeMode,
																				ExceptionRecorder* in_exceptionRecorderRef)
{
	InvalidCleaveSequences producedInvalids;
	// NEWW
	// first, check if we need to invert the normals of each CategorizedLine in each CleaveSequence, in the event that the massManipulationMode of the SPoly is 
	// set to MassManipulationMode::DESTRUCTION

	cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory) ================================================>>>>>> calling constructAndExportCleaveSequences() ", "\n");
	if (cleaveSequenceFactoryLogger.isLoggingSet())
	{
		cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory) lines in pool, prior to merging: ", "\n");
		lineManager.printLineCountsForEachType();
		lineManager.printAllLines();
	}

	// for debug
	originalUnmergedLines = lineManager;

	cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory) >>>> enter any number to continue...", "\n");
	cleaveSequenceFactoryLogger.waitForDebugInput();


	if (in_massManipulationMode == MassManipulationMode::DESTRUCTION)
	{
		//std::cout << "!!!!!!! Destruction poly detected! " << std::endl;
		lineManager.invertEmptyNormalsInContainers();
	}

	// perform merging, but only if the CleaveSequenceMergeMode is MERGE.
	if (in_cleaveSequenceMergeMode == CleaveSequenceMergeMode::MERGE)
	{
		//std::cout << "::::::::::::::::::::::::: Pre-MERGE stats " << std::endl;
		cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory) ::::::::::::::::::::::::: Pre-MERGE stats ", "\n");
		auto borderLinesBegin = in_borderLineArrayRef.begin();
		auto borderLinesEnd = in_borderLineArrayRef.end();
		for (; borderLinesBegin != borderLinesEnd; borderLinesBegin++)
		{
			//std::cout << "Border Line at index " << borderLinesBegin->first << " has " << borderLinesBegin->second.intersectRecorder.records.size() << " records. " << std::endl;
			cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory) Border Line at index ", borderLinesBegin->first, " has ", borderLinesBegin->second.intersectRecorder.records.size(), " records. ", "\n");
		}

		if (cleaveSequenceFactoryLogger.isLoggingSet() == true)
		{
			cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory) starting printing contents of the CategorizedLineGroupMap...", "\n");
			lineManager.printLineCountsForEachType();
			cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory) finished printing contents of the CategorizedLineGroupMap...", "\n");
		}

		if (cleaveSequenceFactoryLogger.isLoggingSet())
		{
			if (mergerDebugLevel != PolyDebugLevel::DEBUG)
			{
				cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory): Notice, merger debug level is not set. ", "\n");
			}
		}

		//std::cout << ">>>> Merger start." << std::endl;
		CategorizedLineMerger merger(this, mergerDebugLevel);

		cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory) ::::::::::::::::::::::::: Post-MERGE stats ", "\n");
		if (cleaveSequenceFactoryLogger.isLoggingSet())
		{
			lineManager.printLineCountsForEachType();
		}
		cleaveSequenceFactoryLogger.waitForDebugInput();

	}

	// find the cycling directions for PARTIAL_BOUND and INTERSECTS_POINT_PRECISE. (will need to eventually include A_SLICE...)
	// if any of the lines can't be determined, don't bother running the CleaveSequences (4/8/2021).
	// This logic will eventually need to be moved to a different location, where only CleaveSequences containing "bad" lines should be dropped,
	// not all CategorizedLines.
	bool areAllLinesValid = determineCyclingDirectionsForCategorizedLines(in_borderLineArrayRef);
	
	if (areAllLinesValid == false)
	{
		std::cout << "(CleaveSequenceFactory): at least one invalid categorized line (undeterminable direction) was found. " << std::endl;
		int someVal = 3;
		std::cout << "Size of cleave map is: " << in_cleaveMapRef->size() << std::endl;
		//while (someVal == 3)
		//{

		//}
	}
	
	else if (areAllLinesValid == true)
	{
		// Typical case 1: only do this if there are partial bound lines or a-sliced lines, and exactly 0 intereceptPointPreciseCount; this is the typical situation.
		if
			(
			(
				(lineManager.getCountOfIntersectionType(IntersectionType::PARTIAL_BOUND) > 0)
				||
				(lineManager.getCountOfIntersectionType(IntersectionType::A_SLICE) > 0)
				||
				(lineManager.getCountOfIntersectionType(IntersectionType::A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE) > 0)
				)
				&&
				(lineManager.getCountOfIntersectionType(IntersectionType::INTERCEPTS_POINT_PRECISE) == 0)
				)
		{
			//std::cout << ">>> Handling typical scenario" << std::endl;

			cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory) >>> Handling typical scenario", "\n");
			cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory) ::::::::::::::::::::::::: Post-MERGE stats (2) ", "\n");
			if (cleaveSequenceFactoryLogger.isLoggingSet())
			{
				lineManager.printLineCountsForEachType();
			}

			producedInvalids = handleScenarioTypical(in_cleaveMapRef, in_exceptionRecorderRef);
		}

		// Special case 1: there is 1 line with a value of INTERCEPTS_POINT_PRECISE.
		else if
			(
				// for a situation in which there is exactly one INTERCEPTS_POINT_PRECISE (this condition will change at a later date.)
			(lineManager.getCountOfIntersectionType(IntersectionType::INTERCEPTS_POINT_PRECISE) == 1)
				)
		{
			//std::cout << ":::: test: " << in_borderLineArrayRef[0].
			//std::cout << ">>> Handling precise scenario" << std::endl;
			cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory) >>> Handling precise scenario", "\n");
			producedInvalids = handleScenarioSingleInterceptsPointPreciseFound(in_cleaveMapRef);
		}
		else if
			(
			(lineManager.getCountOfIntersectionType(IntersectionType::INTERCEPTS_POINT_PRECISE) > 1)
				)
		{
			cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory) >>> Handling multiple precise scenario", "\n");
			producedInvalids = handleScenarioMultipleInterceptsPointPrecise(in_cleaveMapRef);
			//int stopVal = 3;
			//std::cin >> stopVal;
			cleaveSequenceFactoryLogger.waitForDebugInput();
		}
		else
		{
			std::cout << "(CleaveSequenceFactory) !!!!!!!!!!!! Warning, invalid scenario detected, or number of intercept points precise has been reduced to 0...; " << std::endl;
			//std::cout << "!! Number of interceptsPointPrecise: " << interceptsPointPreciseCount << std::endl;
			//int someVal = 3;
			//std::cin >> someVal;
		}

		//std::cout << "================================================>>>>>> End call of constructAndExportCleaveSequences() " << std::endl;
		if (cleaveSequenceFactoryLogger.isLoggingSet())
		{
			cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory) >>> Number of CleaveSequences produced is: ", (*in_cleaveMapRef).size(), "\n");
			cleaveSequenceFactoryLogger.waitForDebugInput();
		}
	}

	//return constructionErrorMessages;
	return producedInvalids;
}

bool CleaveSequenceFactory::determineCyclingDirectionsForCategorizedLines(std::map<int, SPolyBorderLines> in_borderLineArrayRef)
{
	return lineManager.determineCyclingDirections(in_borderLineArrayRef, cleaveSequenceFactoryLogger.getLogLevel());
}

CategorizedLineSearchResult CleaveSequenceFactory::searchForLastPartialBoundLineForSequence(glm::vec3 in_pointToSearch)
{
	return lineManager.searchManagerForLastPartialBoundLineForSequence(in_pointToSearch);
}

CategorizedLineSearchResult CleaveSequenceFactory::searchForInterceptPointPreciseCategorizedLine(glm::vec3 in_pointToSearch)
{
	return lineManager.searchManagerForInterceptPointPreciseCategorizedLine(in_pointToSearch);
}

void CleaveSequenceFactory::printLinesInPool()
{
	lineManager.printAllLines();
}

void CleaveSequenceFactory::printLineCounts()
{
	// NEWW
	std::cout << "!!!!!!~~~~~~ (CSF): calling printLineCountsForEachtype..." << std::endl;
	lineManager.printLineCountsForEachType();
}

void CleaveSequenceFactory::clearLinePools()
{
	lineManager.clearAllLines();
}

InvalidCleaveSequences CleaveSequenceFactory::handleScenarioTypical(std::map<int, CleaveSequence>* in_cleaveMapRef,
																	ExceptionRecorder* in_exceptionRecorderRef)
{
	//bool wasSuccessful = true;
	InvalidCleaveSequences invalids;
	int currentSequenceIndex = 0;

	//std::cout << "!!! Running handleScenarioTypical...." << std::endl;
	//lineManager.printAllLines();

	// NEWW
	// sliced checks.
	bool areNormalsValid = true;
	glm::vec3 emptyVector(0, 0, 0);
	auto slicedBegin = lineManager.getBeginIteratorForType(IntersectionType::A_SLICE);
	auto slicedEnd = lineManager.getEndIteratorForType(IntersectionType::A_SLICE);
	for (; slicedBegin != slicedEnd; slicedBegin++)
	{
		//std::cout << "(CleaveSequenceFactory) ++++++++++ Printing SLICED CategorizedLines: " << std::endl;
		//std::cout << "(CleaveSequenceFactory) Point A: " << slicedBegin->second.line.pointA.x << ", " << slicedBegin->second.line.pointA.y << ", " << slicedBegin->second.line.pointA.z << std::endl;
		//std::cout << "(CleaveSequenceFactory) Point B: " << slicedBegin->second.line.pointB.x << ", " << slicedBegin->second.line.pointB.y << ", " << slicedBegin->second.line.pointB.z << std::endl;
		cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory) ++++++++++ Printing SLICED CategorizedLines: ", "\n");
		cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory) Point A: ", slicedBegin->second.line.pointA.x, ", ", slicedBegin->second.line.pointA.y, ", ", slicedBegin->second.line.pointA.z, "\n");
		cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory) Point B: ", slicedBegin->second.line.pointB.x, ", ", slicedBegin->second.line.pointB.y, ", ", slicedBegin->second.line.pointB.z, "\n");
		if (slicedBegin->second.emptyNormal == emptyVector)
		{
			areNormalsValid = false;
			std::cout << "!!!!! NOTICE: A_SLICE normal detected as invalid." << std::endl;
		}
	}

	if (cleaveSequenceFactoryLogger.isLoggingSet())
	{
		cleaveSequenceFactoryLogger.log("(CleaveSequenceFactorY): Line pool, before run of handleScenarioTypical: ", "\n");
		lineManager.printAllLines();
	}

	// only continue if areNormalsValid is true.
	if (areNormalsValid == true)
	{
		int typicalWhileIteration = 0;
		while (lineManager.getCountOfIntersectionType(IntersectionType::PARTIAL_BOUND) > 0)	// do this until all partial_bound lines have been accounted for. 
		{
			cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory): BEGIN, partialBoundCount > 0 --> Number of non-bound lines in map is:", lineManager.getCountOfIntersectionType(IntersectionType::NON_BOUND), "\n");

			cleaveSequenceMapRef = in_cleaveMapRef;									// set the map reference that we will export results to.

			auto partialBoundMapBegin = lineManager.getBeginIteratorForType(IntersectionType::PARTIAL_BOUND);					// get the first line in the partial bound map
			CategorizedLine* partialBoundLineRef = &partialBoundMapBegin->second;	// get a ref to the line

			if (cleaveSequenceFactoryLogger.isLoggingSet())
			{
				cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory) Current partial bound line stats are:", "\n");
				cleaveSequenceFactoryLogger.log("Point A: ", partialBoundLineRef->line.pointA.x, ", ", partialBoundLineRef->line.pointA.y, ", ", partialBoundLineRef->line.pointA.z, "\n");
				cleaveSequenceFactoryLogger.log("Point B: ", partialBoundLineRef->line.pointB.x, ", ", partialBoundLineRef->line.pointB.y, ", ", partialBoundLineRef->line.pointB.z, "\n");

			}

			int firstLineID = partialBoundMapBegin->first;							// store the ID of the first line (for removal later)
			CleaveSequence newSequence;												// the new line sequence that will eventually be inserted back into the referenced SPoly
			//insertFirstPartialBoundLineForSequence(&newSequence, firstLineID);		// insert the first partial bound line we find
			newSequence.insertFirstLine(lineManager.fetchAndRemoveLineAtIndex(IntersectionType::PARTIAL_BOUND, firstLineID));	// insert the first partial bound line we find
			glm::vec3 firstPointToSearch = newSequence.fetchPointToSearch();		// get the searchable point from the first partial bound line we found in the previous step

			cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory): Next point to search is: ", firstPointToSearch.x, ", ", firstPointToSearch.y, ", ", firstPointToSearch.z, "\n");

			//std::cout << "!!! Initial point to search is: " << firstPointToSearch.x << ", " << firstPointToSearch.y << ", " << firstPointToSearch.z << std::endl;


			// first, work with the partially bound lines. Get the first available partial bound line in the map, scan for linking nonbound lines, until no more linking lines are found.
			// Then, scan the partialBoundMap until the ending partially bound line is found (the B point of this partial line will equal the B point of a linked nonbound line). 
			// When this is done, take all the lines invovled (both nonbound and partial bound) and put them into a new CleaveSequence. Remmove their original copies from the appropriate maps --
			// partial bound lines will be removed from partialBoundMap (and the counter decremented) and the nonbound lines will be removed from the nonboundMap (also decrementing here)
			bool continueSearch = true;
			//CategorizedLineSearchResult result = checkForNextNonboundLine(firstPointToSearch);	// search for the first point.
			CategorizedLineSearchResult result = lineManager.checkManagerForNextNonboundLine(firstPointToSearch);	// search for the next nonbound line, via the lineManager
			cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory): MIDDLE, partialBoundCount > 0 --> Number of non-bound lines in map is:", lineManager.getCountOfIntersectionType(IntersectionType::NON_BOUND), "\n");
			if (result.wasFound == true)		// insert the first categorized line into the sequence, if it was found:
			{
				cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory): Found initial non-bound line to insert.", "\n");
				cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory): Non-bound point A: ", result.returnLine.line.pointA.x, ", ", result.returnLine.line.pointA.y, ", ", result.returnLine.line.pointA.z, "\n");
				cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory): Non-bound point B: ", result.returnLine.line.pointB.x, ", ", result.returnLine.line.pointB.y, ", ", result.returnLine.line.pointB.z, "\n");

				newSequence.insertNonboundLine(result.returnLine);	// insert the fetched line into the sequence
				bool continueFlag = true;							// check for the next line, at least once.
				while (continueFlag == true)	// loop until this is false.
				{
					glm::vec3 nextPointToSearch = newSequence.fetchPointToSearch();
					cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory): Next point to search is: ", nextPointToSearch.x, ", ", nextPointToSearch.y, ", ", nextPointToSearch.z, "\n");
					//CategorizedLineSearchResult nextResult = checkForNextNonboundLine(nextPointToSearch);
					CategorizedLineSearchResult nextResult = lineManager.checkManagerForNextNonboundLine(nextPointToSearch);	// search for the next nonbound line, via the lineManager
					if (nextResult.wasFound == false)
					{
						continueFlag = false;	// end the loop.
					}
					else if (nextResult.wasFound == true)
					{
						cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory): Found next non-bound line to insert.", "\n");
						newSequence.insertNonboundLine(nextResult.returnLine);
					}
				}
			}

			// once the search for partially bound lines is done, look for the ending partially bound line for the sequence.
			glm::vec3 lastPointToSearch = newSequence.fetchPointToSearch();
			//std::cout << "### Current last point to search is: " << lastPointToSearch.x << ", " << lastPointToSearch.y << ", " << lastPointToSearch.z << std::endl;
			CategorizedLineSearchResult finalResult = searchForLastPartialBoundLineForSequence(lastPointToSearch);
			if (finalResult.wasFound == true)
			{
				//std::cout << "Final partial bound line found! Inserting final line... !!" << std::endl;
				newSequence.insertLastLine(finalResult.returnLine);
				newSequence.sequenceStatus = CleaveSequenceStatus::COMPLETE; // mark it as complete

				if (finalResult.backupOptionFlag == true)
				{
					std::cout << "(CleaveSequenceFactory::handleScenarioTypical) !!! backupOptionFlag detected as true. " << std::endl;
					newSequence.printCategorizedLines();
					newSequence.specialLogicUsedFlag = true;
				}
			}
			else
			{
				//std::cout << "!! Final partial bound line NOT FOUND! " << std::endl;
				newSequence.sequenceStatus = CleaveSequenceStatus::INCOMPLETE; // mark it as complete

				std::cout << "(CleaveSequenceFactory) Warning, CleaveSequence is INCOMPLETE. " << std::endl;
				std::cout << "(CleaveSequenceFactory) Original set of unmerged lines is: " << std::endl;
				originalUnmergedLines.printAllLines();
				

				std::cout << "(CleaveSequenceFactory) Last point to search value is: " << lastPointToSearch.x << ", " << lastPointToSearch.y << ", " << lastPointToSearch.z << std::endl;

				std::cout << "(CleaveSequenceFactory) Remaining lines in line manager are: " << std::endl;
				lineManager.printAllLines();

				std::cout << "(CleaveSequenceFactory) Optional orientation value is: ";
				switch (optionalFactoryOrientation)
				{
						case (BoundaryOrientation::NONE): { std::cout << "NONE"; break; };
						case (BoundaryOrientation::NEG_X): { std::cout << "NEG_X"; break; };
						case (BoundaryOrientation::NEG_Y): { std::cout << "NEG_Y"; break; };
						case (BoundaryOrientation::NEG_Z): { std::cout << "NEG_Z"; break; };
						case (BoundaryOrientation::POS_X): { std::cout << "POS_X"; break; };
						case (BoundaryOrientation::POS_Y): { std::cout << "POS_Y"; break; };
						case (BoundaryOrientation::POS_Z): { std::cout << "POS_Z"; break; };
				}
				std::cout << std::endl;


				std::cout << ":(CleaveSequenceFactory) Last point to search was: " << lastPointToSearch.x << ", " << lastPointToSearch.y << ", " << lastPointToSearch.z << std::endl;
				std::cout << "(CleaveSequenceFactory)  Lines are: " << std::endl;


				hasBadProduction = true;
				newSequence.printCategorizedLines();

				invalids.insertCleaveSequenceAtKey(currentSequenceIndex, newSequence);

				// --Record exception, EXCEPTION_INVALID_TYPICAL_SEQUENCE here; note that this may actually insert the exception twice
				// because of the while loop.

				//std::cout << "+++++++++++++++ entering infinite while for debug testing (6/2/2021) --> handled by EXCEPTION_INVALID_TYPICAL_SEQUENCE." << std::endl;
				//std::cout << "+++++++++++++++ exception count is: " << exceptionCount << std::endl;
				//exceptionCount++;
				//int someVal = 3;
				//std::cin >> someVal;

				ExceptionRecord sequenceException(ExceptionRecordType::EXCEPTION_INVALID_TYPICAL_SEQUENCE, 
												writeOutCategorizedLines(&newSequence, typicalWhileIteration));
				in_exceptionRecorderRef->insertException(sequenceException);

			}
			//std::cout << "## Remaining number of partial bounds: " << partialboundCount << std::endl;


			// lastly, if the newSequence is marked as "complete" move it to the referenced sequence. Otherwise, discard it.
			if (newSequence.sequenceStatus == CleaveSequenceStatus::COMPLETE)
			{
				int cleaveMapRefSize = int((*in_cleaveMapRef).size());
				//std::cout << "!! Inserting new cleave sequence at index: " << cleaveMapRefSize << std::endl;
				(*in_cleaveMapRef)[cleaveMapRefSize] = newSequence;	// insert the sequence.
				//std::cout << "Map size is now: " << cleaveMapRefSize << std::endl;
			}

			cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory) ++++++++++ Finished 1 pass of a CategorizedLine. ", "\n");

			currentSequenceIndex++;
			typicalWhileIteration++;
		}

		while (lineManager.getCountOfIntersectionType(IntersectionType::A_SLICE) > 0)
		{
			//std::cout << "(CleaveSequenceFactory)  STOP! It's hammer time. " << std::endl;
			cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory)  STOP! It's hammer time. ", "\n");

			cleaveSequenceMapRef = in_cleaveMapRef;									// set the map reference that we will export results to.
			auto aslicedMapBegin = lineManager.getBeginIteratorForType(IntersectionType::A_SLICE);					// get the first line in the a_slice map
			int firstLineID = aslicedMapBegin->first;							// store the ID of the first line (for removal later)
			CleaveSequence newSequence;
			//insertASliceLineForSequence(&newSequence, firstLineID);
			newSequence.insertFirstLine(lineManager.fetchAndRemoveLineAtIndex(IntersectionType::A_SLICE, firstLineID));
			int cleaveMapRefSize = int((*in_cleaveMapRef).size());
			(*in_cleaveMapRef)[cleaveMapRefSize] = newSequence;	// insert the sequence.

			//newSequence.printCategorizedLines();
			//int someVal = 3;
			//std::cin >> someVal;

			if (cleaveSequenceFactoryLogger.isLoggingSet() == true)
			{
				cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory) >>>> started printing out of produced A_SLICE categorized lines.", "\n");
				newSequence.printCategorizedLines();
				cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory) >>>> finished printing out of produced A_SLICE categorized lines.", "\n");
				cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory) number of CleaveSequences is now: ", int((*in_cleaveMapRef).size()), "\n");
				cleaveSequenceFactoryLogger.waitForDebugInput();
			}


		}

		while (lineManager.getCountOfIntersectionType(IntersectionType::A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE) > 0)
		{
			cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory)  STOP! It's hammer time....for an aslicedSingleInterceptsPointPrecise!", "\n");
			//std::cout << "(CleaveSequenceFactory)  STOP! It's hammer time....for an aslicedSingleInterceptsPointPrecise!" << std::endl;
			auto aslicedSingleInterceptsPointPreciseFirst = lineManager.getBeginIteratorForType(IntersectionType::A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE);
			int firstLineID = aslicedSingleInterceptsPointPreciseFirst->first;
			CleaveSequence newSequence;
			//insertASliceSingleInterceptsPointPreciseForSequence(&newSequence, firstLineID);
			//newSequence.insertFirstLine(fetchAndRemoveASliceSingleInterceptsPointPrecise(firstLineID));
			newSequence.insertFirstLine(lineManager.fetchAndRemoveLineAtIndex(IntersectionType::A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE, firstLineID));
			int cleaveMapRefSize = int((*in_cleaveMapRef).size());
			(*in_cleaveMapRef)[cleaveMapRefSize] = newSequence;	// insert the sequence.
		}
	}

	//return typicalErrorMessages;
	return invalids;
}

Message CleaveSequenceFactory::writeOutCategorizedLines(CleaveSequence* in_currentCleaveSequence, int in_currentIteration)
{
	// The message everything will go into.
	Message writtenLines(MessageType::CLEAVESEQUENCE_CATEGORIZED_LINES);

	// The first string will containt the orientation of the SPoly that the exception occurred in.
	std::string debugStringBase = "\tAn ExceptionRecordType::EXCEPTION_INVALID_TYPICAL_SEQUENCE was detected during generation of CleaveSequences, in the PARTIAL_BOUND while loop at iteration " + std::to_string(in_currentIteration) + " in an SPoly having an orientation of ";
	auto orientationString = IndependentUtils::getBoundaryOrientationString(optionalFactoryOrientation);
	std::string debugString = debugStringBase + orientationString;
	writtenLines.insertString(debugString);

	// Read all categorized lines from both the lineManager, and the current CleaveSequence.
	// Get lineManager lines, and line count.
	auto fetchedManagerLines = lineManager.fetchAllLines(); // fetch CategorizedLines from lineManager 
	int totalManagerLines = fetchedManagerLines.size();
	writtenLines.insertInt(totalManagerLines);			// the first int of the message should be the number of lines from the lineManager.

	// Get a pointer to the CleaveSequence lines, and the total size of those.
	auto cleaveSequenceMapPtr = &in_currentCleaveSequence->cleavingLines;
	int totalCleaveSequenceLines = cleaveSequenceMapPtr->size();
	writtenLines.insertInt(totalCleaveSequenceLines);

	// Now, cycle through each CategorizedLine from the lineManager. Make sure to insert two tabs.
	writtenLines.insertString("\t\tremaining lines in lineManager: ");
	for (auto& managerLines : fetchedManagerLines)
	{
		// before writing out the string, adjust the float precision to 2 for easier readability.
		writtenLines.insertString(managerLines.writeLineToString());
	}

	// Then, the cleaveSequence lines. Make sure to insert two tabs.
	writtenLines.insertString("\t\tlines inserted into CleaveSequence: ");
	for (auto& cleaveSequenceLines : in_currentCleaveSequence->cleavingLines)
	{
		// before writing out the string, adjust the float precision to 2 for easier readability.
		writtenLines.insertString(cleaveSequenceLines.second.writeLineToString());
	}

	return writtenLines;
}

InvalidCleaveSequences CleaveSequenceFactory::handleScenarioSingleInterceptsPointPreciseFound(std::map<int, CleaveSequence>* in_cleaveMapRef)
{
	std::cout << "!!! Running handleScenarioSingleInterceptsPointPreciseFound...." << std::endl;
	//bool wasSuccessful = true;
	//MessageContainer preciseErrorMessages;
	InvalidCleaveSequences invalids;

	// NEWW
	glm::vec3 lastPointToSearch;
	while (lineManager.getCountOfIntersectionType(IntersectionType::PARTIAL_BOUND) > 0)	// do this until all partial_bound lines have been accounted for. 
	{
		cleaveSequenceMapRef = in_cleaveMapRef;									// set the map reference that we will export results to.

		auto partialBoundMapBegin = lineManager.getBeginIteratorForType(IntersectionType::PARTIAL_BOUND);					// get the first line in the partial bound map
		CategorizedLine* partialBoundLineRef = &partialBoundMapBegin->second;	// get a ref to the line
		int firstLineID = partialBoundMapBegin->first;							// store the ID of the first line (for removal later)
		CleaveSequence newSequence;												// the new line sequence that will eventually be inserted back into the referenced SPoly
		//insertFirstPartialBoundLineForSequence(&newSequence, firstLineID);		// insert the first partial bound line we find
		newSequence.insertFirstLine(lineManager.fetchAndRemoveLineAtIndex(IntersectionType::PARTIAL_BOUND, firstLineID));  // insert the first partial bound line we find
		glm::vec3 firstPointToSearch = newSequence.fetchPointToSearch();		// get the searchable point from the first partial bound line we found in the previous step
		//std::cout << "!!! Initial point to search is: " << firstPointToSearch.x << ", " << firstPointToSearch.y << ", " << firstPointToSearch.z << std::endl;


		// first, work with the partially bound lines. Get the first available partial bound line in the map, scan for linking nonbound lines, until no more linking lines are found.
		// Then, scan the partialBoundMap until the ending partially bound line is found (the B point of this partial line will equal the B point of a linked nonbound line). 
		// When this is done, take all the lines invovled (both nonbound and partial bound) and put them into a new CleaveSequence. Remmove their original copies from the appropriate maps --
		// partial bound lines will be removed from partialBoundMap (and the counter decremented) and the nonbound lines will be removed from the nonboundMap (also decrementing here)
		bool continueSearch = true;
		//CategorizedLineSearchResult result = checkForNextNonboundLine(firstPointToSearch);	// search for the first point.
		CategorizedLineSearchResult result = lineManager.checkManagerForNextNonboundLine(firstPointToSearch);	// search for the next nonbound line, via the lineManager
		if (result.wasFound == true)		// insert the first categorized line into the sequence, if it was found:
		{
			newSequence.insertNonboundLine(result.returnLine);	// insert the fetched line into the sequence
			bool continueFlag = true;							// check for the next line, at least once.
			while (continueFlag == true)	// loop until this is false.
			{
				glm::vec3 nextPointToSearch = newSequence.fetchPointToSearch();
				//CategorizedLineSearchResult nextResult = checkForNextNonboundLine(nextPointToSearch);
				CategorizedLineSearchResult nextResult = lineManager.checkManagerForNextNonboundLine(nextPointToSearch);	// search for the next nonbound line, via the lineManager
				if (nextResult.wasFound == false)
				{
					continueFlag = false;	// end the loop.
				}
				else if (nextResult.wasFound == true)
				{
					newSequence.insertNonboundLine(nextResult.returnLine);
				}
			}
		}

		// once the search for non bound lines is done, look for the ending partially bound line for the sequence.
		lastPointToSearch = newSequence.fetchPointToSearch();
		//std::cout << "### Current last point to search is: " << lastPointToSearch.x << ", " << lastPointToSearch.y << ", " << lastPointToSearch.z << std::endl;
		CategorizedLineSearchResult finalResult = searchForLastPartialBoundLineForSequence(lastPointToSearch);
		if (finalResult.wasFound == true)
		{
			//std::cout << "Final partial bound line found! Inserting final line... !!" << std::endl;
			newSequence.insertLastLine(finalResult.returnLine);
			newSequence.sequenceStatus = CleaveSequenceStatus::COMPLETE; // mark it as complete
		}
		else
		{
			//std::cout << "!! Final partial bound line NOT FOUND! " << std::endl;
			//newSequence.sequenceStatus = CleaveSequenceStatus::INCOMPLETE; // mark it as complete

			//std::cout << "Warning, a corresponding PARTIAL_BOUND line was not found for this line; checking for INTERCEPT_POINT_PRECISE: " << std::endl;

			//std::cout << "::: Lines are: " << std::endl;
			//newSequence.printCategorizedLines();

			//std::cout << ">>>> Current last point to search for against the INTERCEPTS_POINT_PRECISE IS: " << lastPointToSearch.x << ", " << lastPointToSearch.y << ", " << lastPointToSearch.z << std::endl;
			//CategorizedLineSearchResult findInterceptPointPreciseResult = searchForInterceptPointPreciseCategorizedLine(lastPointToSearch, in_borderLineArrayRef);
			CategorizedLineSearchResult findInterceptPointPreciseResult = searchForInterceptPointPreciseCategorizedLine(lastPointToSearch);
			//std::cout << ">>>> Found intercept point precise result... " << std::endl;
			if (findInterceptPointPreciseResult.wasFound == true)
			{
				// before we insert the line, the pointB on border value for this new line must be equal to the pointA border value in the first part of the sequence.
				// this is because 
				auto firstLineInSequence = newSequence.cleavingLines.begin();
				//int borderIDForInterceptPointPrecise = firstLineInSequence->second.line.pointABorder;
				//std::cout << ">>> :::: Border value to set is: " << borderIDForInterceptPointPrecise << std::endl;
				//findInterceptPointPreciseResult.returnLine.line.pointBBorder = borderIDForInterceptPointPrecise;

				//std::cout << "Final partial bound line found! Inserting final line... !!" << std::endl;
				newSequence.insertLastLine(findInterceptPointPreciseResult.returnLine);
				newSequence.sequenceStatus = CleaveSequenceStatus::COMPLETE; // mark it as complete
			}
			//int someVal = 3;
			//std::cin >> someVal;
		}

		// lastly, if the newSequence is marked as "complete" move it to the referenced sequence. Otherwise, discard it.
		if (newSequence.sequenceStatus == CleaveSequenceStatus::COMPLETE)
		{
			int cleaveMapRefSize = int((*in_cleaveMapRef).size());
			//std::cout << "(CleaveSequenceFactory)  !! Inserting new cleave sequence at index: " << cleaveMapRefSize << std::endl;
			cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory) !! Inserting new cleave sequence at index: ", cleaveMapRefSize, "\n");
			(*in_cleaveMapRef)[cleaveMapRefSize] = newSequence;	// insert the sequence.
			//std::cout << "(CleaveSequenceFactory)  Map size is now: " << cleaveMapRefSize << std::endl;
			cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory)  Map size is now: ", cleaveMapRefSize, "\n");
		}
	}

	if (lineManager.getCountOfIntersectionType(IntersectionType::A_SLICE) > 0)
	{
		std::cout << "(CleaveSequenceFactory)  !!!!#### NOTICE: there is at least one A_SLICE in the pools...continue? " << std::endl;
		std::cout << "(CleaveSequenceFactory)  STOP! It's hammer time. " << std::endl;
		cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory)  !!!!#### NOTICE: there is at least one A_SLICE in the pools...continue? ", "\n");
		cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory)  STOP! It's hammer time. ", "\n");

		cleaveSequenceMapRef = in_cleaveMapRef;									// set the map reference that we will export results to.
		auto aslicedMapBegin = lineManager.getBeginIteratorForType(IntersectionType::A_SLICE);					// get the first line in the a_slice map
		int firstLineID = aslicedMapBegin->first;							// store the ID of the first line (for removal later)
		CleaveSequence newSequence;
		//insertASliceLineForSequence(&newSequence, firstLineID);
		newSequence.insertFirstLine(lineManager.fetchAndRemoveLineAtIndex(IntersectionType::A_SLICE, firstLineID));
		int cleaveMapRefSize = int((*in_cleaveMapRef).size());

		//std::cout << "(CleaveSequenceFactory)  Index for new cleaveSequence is: " << cleaveMapRefSize << std::endl;
		cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory)  Index for new cleaveSequence is: ", cleaveMapRefSize, "\n");

		(*in_cleaveMapRef)[cleaveMapRefSize] = newSequence;	// insert the sequence.

		if (cleaveSequenceFactoryLogger.isLoggingSet() == true)
		{
			newSequence.printCategorizedLines();
		}

		cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory) Enter any number to continue... ", "\n");
		cleaveSequenceFactoryLogger.waitForDebugInput();
		//int noticeVal = 3;
		//std::cin >> noticeVal;
	}

	else if (lineManager.getCountOfIntersectionType(IntersectionType::INTERCEPTS_POINT_PRECISE) == 1)
	{
		//std::cout << "(CleaveSequenceFactory): handling case of a single INTERCEPTS_POINT_PRECISE. " << std::endl;
	}

	return invalids;
}

InvalidCleaveSequences CleaveSequenceFactory::handleScenarioMultipleInterceptsPointPrecise(std::map<int, CleaveSequence>* in_cleaveMapRef)
{
	std::cout << "!!! Running handleScenarioMultipleInterceptsPointPrecise..." << std::endl;
	//bool wasSuccessful = true;
	//MessageContainer multiplePreciseErrorMessages;
	InvalidCleaveSequences invalids;

	// NEWW
	glm::vec3 lastPointToSearch;
	while (lineManager.getCountOfIntersectionType(IntersectionType::INTERCEPTS_POINT_PRECISE) > 0)
	{
		// get the first intercepts point precise that will be the first line in the CleaveSequence
		cleaveSequenceMapRef = in_cleaveMapRef;
		auto preciseMapBegin = lineManager.getBeginIteratorForType(IntersectionType::INTERCEPTS_POINT_PRECISE);
		int firstLineID = preciseMapBegin->first;							// store the ID of the first line (for removal later)
		CleaveSequence newSequence;
		//insertFirstInterceptsPointPreciseForSequence(&newSequence, firstLineID);
		//newSequence.insertFirstLine(fetchAndRemoveInterceptPointPrecise(firstLineID));
		newSequence.insertFirstLine(lineManager.fetchAndRemoveLineAtIndex(IntersectionType::INTERCEPTS_POINT_PRECISE, firstLineID));
		glm::vec3 firstPointToSearch = newSequence.fetchPointToSearch();

		// search for any nonbound lines
		bool continueSearch = true;
		//CategorizedLineSearchResult result = checkForNextNonboundLine(firstPointToSearch);	// search for the first point.
		CategorizedLineSearchResult result = lineManager.checkManagerForNextNonboundLine(firstPointToSearch);	// search for the next nonbound line, via the lineManager
		if (result.wasFound == true)		// insert the first categorized line into the sequence, if it was found:
		{
			newSequence.insertNonboundLine(result.returnLine);	// insert the fetched line into the sequence
			bool continueFlag = true;							// check for the next line, at least once.
			while (continueFlag == true)	// loop until this is false.
			{
				glm::vec3 nextPointToSearch = newSequence.fetchPointToSearch();
				//CategorizedLineSearchResult nextResult = checkForNextNonboundLine(nextPointToSearch);
				CategorizedLineSearchResult nextResult = lineManager.checkManagerForNextNonboundLine(nextPointToSearch);	// search for the next nonbound line, via the lineManager
				if (nextResult.wasFound == false)
				{
					continueFlag = false;	// end the loop.
				}
				else if (nextResult.wasFound == true)
				{
					newSequence.insertNonboundLine(nextResult.returnLine);
				}
			}
		}

		// search for final intercepts point precise, if it exists
		lastPointToSearch = newSequence.fetchPointToSearch();
		CategorizedLineSearchResult findInterceptPointPreciseResult = searchForInterceptPointPreciseCategorizedLine(lastPointToSearch);
		if (findInterceptPointPreciseResult.wasFound == true)
		{
			//findInterceptPointPreciseResult.returnLine.line.swapToA();	// no swap required, handled by searchForInterceptPointPreciseCategorizedLine
			newSequence.insertLastLine(findInterceptPointPreciseResult.returnLine);
			newSequence.sequenceStatus = CleaveSequenceStatus::COMPLETE; // mark it as complete
		}

		// lastly, if the newSequence is marked as "complete" move it to the referenced sequence. Otherwise, discard it.
		if (newSequence.sequenceStatus == CleaveSequenceStatus::COMPLETE)
		{
			int cleaveMapRefSize = int((*in_cleaveMapRef).size());
			std::cout << "!! Inserting new cleave sequence at index: " << cleaveMapRefSize << std::endl;
			(*in_cleaveMapRef)[cleaveMapRefSize] = newSequence;	// insert the sequence.
			//std::cout << "Map size is now: " << cleaveMapRefSize << std::endl;
		}
		else
		{
			std::cout << "!!! NOTICE, double precise point solution not found. " << std::endl;
		}

		newSequence.printCategorizedLines();

		//std::cout << "(CleaveSequenceFactory)  !! End of handle multiple precise loop tick. Continue? " << std::endl;
		//int endTest = 3;
		//std::cin >> endTest;
	}

	//return multiplePreciseErrorMessages;
	return invalids;
}