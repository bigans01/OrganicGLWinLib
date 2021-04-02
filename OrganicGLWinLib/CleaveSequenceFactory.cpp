#include "stdafx.h"
#include "CleaveSequenceFactory.h"

void CleaveSequenceFactory::addCategorizedLine(CategorizedLine in_categorizedLine)
{
	lineManager.insertLineAndReturnInsertedIndex(in_categorizedLine); // new test for 4/2/2021, for refactor.
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

void CleaveSequenceFactory::insertPartialBoundLine(CategorizedLine in_line)
{

	lineManager.insertLineAndReturnInsertedIndex(in_line); // new test for 4/2/2021, for refactor.
}

void CleaveSequenceFactory::insertAslicedLine(CategorizedLine in_line)
{
	lineManager.insertLineAndReturnInsertedIndex(in_line); // new test for 4/2/2021, for refactor.
}

void CleaveSequenceFactory::insertAslicedSingleInterceptsPointPrecise(CategorizedLine in_line)
{
	lineManager.insertLineAndReturnInsertedIndex(in_line); // new test for 4/2/2021, for refactor.
}

void CleaveSequenceFactory::insertInterceptsPointPrecise(CategorizedLine in_line)
{
	lineManager.insertLineAndReturnInsertedIndex(in_line); // new test for 4/2/2021, for refactor.
}

CategorizedLine CleaveSequenceFactory::fetchAndRemoveNonbound(int in_fetchIndex)
{
	CategorizedLine fetchedLine = lineManager.fetchAndRemoveLineAtIndex(IntersectionType::NON_BOUND, in_fetchIndex);
	return fetchedLine;
}

CategorizedLine CleaveSequenceFactory::fetchAndRemoveNonboundWithGroupMapLocationPush(int in_fetchIndex, std::vector<CategorizedLineGroupLocation>* in_categorizedLineGroupLocationVectorRef)
{
	CategorizedLine fetchedLine = lineManager.fetchAndRemoveWithGroupMapLocationPush(IntersectionType::NON_BOUND, in_fetchIndex, in_categorizedLineGroupLocationVectorRef);
	return fetchedLine;
}

CategorizedLine CleaveSequenceFactory::fetchAndRemovePartialBound(int in_fetchIndex)
{
	CategorizedLine fetchedLine = lineManager.fetchAndRemoveLineAtIndex(IntersectionType::PARTIAL_BOUND, in_fetchIndex);
	return fetchedLine;
}

CategorizedLine CleaveSequenceFactory::fetchAndRemovePartialBoundWithGroupMapLocationPush(int in_fetchIndex, std::vector<CategorizedLineGroupLocation>* in_categorizedLineGroupLocationVectorRef)
{
	CategorizedLine fetchedLine = lineManager.fetchAndRemoveWithGroupMapLocationPush(IntersectionType::PARTIAL_BOUND, in_fetchIndex, in_categorizedLineGroupLocationVectorRef);
	return fetchedLine;
}

CategorizedLine CleaveSequenceFactory::fetchAndRemoveASlice(int in_fetchIndex)
{
	CategorizedLine fetchedLine = lineManager.fetchAndRemoveLineAtIndex(IntersectionType::A_SLICE, in_fetchIndex);
	return fetchedLine;
}

CategorizedLine CleaveSequenceFactory::fetchAndRemoveASliceWithGroupMapLocationPush(int in_fetchIndex, std::vector<CategorizedLineGroupLocation>* in_categorizedLineGroupLocationVectorRef)
{
	CategorizedLine fetchedLine = lineManager.fetchAndRemoveWithGroupMapLocationPush(IntersectionType::A_SLICE, in_fetchIndex, in_categorizedLineGroupLocationVectorRef);
	return fetchedLine;
}

CategorizedLine CleaveSequenceFactory::fetchAndRemoveASliceSingleInterceptsPointPrecise(int in_fetchIndex)
{
	CategorizedLine fetchedLine = lineManager.fetchAndRemoveLineAtIndex(IntersectionType::A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE, in_fetchIndex);
	return fetchedLine;
}


CategorizedLine CleaveSequenceFactory::fetchAndRemoveASliceSingleInterceptsPointPreciseWithGroupMapLocationPush(int in_fetchIndex, std::vector<CategorizedLineGroupLocation>* in_categorizedLineGroupLocationVectorRef)
{
	CategorizedLine fetchedLine = lineManager.fetchAndRemoveWithGroupMapLocationPush(IntersectionType::A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE, in_fetchIndex, in_categorizedLineGroupLocationVectorRef);
	return fetchedLine;
}

CategorizedLine CleaveSequenceFactory::fetchAndRemoveInterceptPointPrecise(int in_fetchIndex)
{
	CategorizedLine fetchedLine = lineManager.fetchAndRemoveLineAtIndex(IntersectionType::INTERCEPTS_POINT_PRECISE, in_fetchIndex);
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

void CleaveSequenceFactory::loadCategorizedLineMapReferencesIntoQuatPointsExcludeEmptyNormals(QuatRotationPoints* in_quatRotationPointsRef)
{
	lineManager.loadCategorizedLinePoints(in_quatRotationPointsRef);
}

void CleaveSequenceFactory::loadCategorizedLineEmptyNormalsIntoQuatPoints(QuatRotationPoints* in_quatRotationPointsRef)
{
	lineManager.loadCategorizedLineEmptyNormals(in_quatRotationPointsRef);
}



void CleaveSequenceFactory::constructAndExportCleaveSequences(std::map<int, CleaveSequence>* in_cleaveMapRef, std::map<int, SPolyBorderLines> in_borderLineArrayRef, MassManipulationMode in_massManipulationMode, CleaveSequenceMergeMode in_cleaveSequenceMergeMode)
{
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
	determineCyclingDirectionsForCategorizedLines(in_borderLineArrayRef);

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

		handleScenarioTypical(in_cleaveMapRef);
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
		handleScenarioSingleInterceptsPointPreciseFound(in_cleaveMapRef);
	}
	else if
	(
		(lineManager.getCountOfIntersectionType(IntersectionType::INTERCEPTS_POINT_PRECISE) > 1)
	)
	{
		cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory) >>> Handling multiple precise scenario", "\n");
		handleScenarioMultipleInterceptsPointPrecise(in_cleaveMapRef);
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

void CleaveSequenceFactory::determineCyclingDirectionsForCategorizedLines(std::map<int, SPolyBorderLines> in_borderLineArrayRef)
{
	lineManager.determineCyclingDirections(in_borderLineArrayRef, cleaveSequenceFactoryLogger.getLogLevel());
}

void CleaveSequenceFactory::insertFirstPartialBoundLineForSequence(CleaveSequence* in_cleaveSequenceRef, int in_lineIndex)
{
	in_cleaveSequenceRef->insertFirstLine(fetchAndRemovePartialBound(in_lineIndex));
}

void CleaveSequenceFactory::insertASliceLineForSequence(CleaveSequence* in_cleaveSequenceRef, int in_lineIndex)
{
	in_cleaveSequenceRef->insertFirstLine(fetchAndRemoveASlice(in_lineIndex));
}

void CleaveSequenceFactory::insertASliceSingleInterceptsPointPreciseForSequence(CleaveSequence* in_cleaveSequenceRef, int in_lineIndex)
{
	in_cleaveSequenceRef->insertFirstLine(fetchAndRemoveASliceSingleInterceptsPointPrecise(in_lineIndex));
}

void CleaveSequenceFactory::insertFirstInterceptsPointPreciseForSequence(CleaveSequence* in_cleaveSequenceRef, int in_lineIndex)
{
	in_cleaveSequenceRef->insertFirstLine(fetchAndRemoveInterceptPointPrecise(in_lineIndex));
}

void CleaveSequenceFactory::invertAllEmptyNormals()
{
	lineManager.invertEmptyNormalsInContainers();
}

CategorizedLineSearchResult CleaveSequenceFactory::searchForLastPartialBoundLineForSequence(glm::vec3 in_pointToSearch)
{
	return lineManager.searchManagerForLastPartialBoundLineForSequence(in_pointToSearch);
}

CategorizedLineSearchResult CleaveSequenceFactory::searchForInterceptPointPreciseCategorizedLine(glm::vec3 in_pointToSearch)
{
	return lineManager.searchManagerForInterceptPointPreciseCategorizedLine(in_pointToSearch);
}

CategorizedLineSearchResult CleaveSequenceFactory::checkForNextNonboundLine(glm::vec3 in_pointToSearch)
{
	return lineManager.checkManagerForNextNonboundLine(in_pointToSearch);
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

void CleaveSequenceFactory::handleScenarioTypical(std::map<int, CleaveSequence>* in_cleaveMapRef)
{
	// NEWW
	// sliced checks.
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

	}

	if (cleaveSequenceFactoryLogger.isLoggingSet())
	{
		cleaveSequenceFactoryLogger.log("(CleaveSequenceFactorY): Line pool, before run of handleScenarioTypical: ", "\n");
		lineManager.printAllLines();
	}

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
		insertFirstPartialBoundLineForSequence(&newSequence, firstLineID);		// insert the first partial bound line we find
		glm::vec3 firstPointToSearch = newSequence.fetchPointToSearch();		// get the searchable point from the first partial bound line we found in the previous step

		cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory): Next point to search is: ", firstPointToSearch.x, ", ", firstPointToSearch.y, ", ", firstPointToSearch.z, "\n");

		//std::cout << "!!! Initial point to search is: " << firstPointToSearch.x << ", " << firstPointToSearch.y << ", " << firstPointToSearch.z << std::endl;


		// first, work with the partially bound lines. Get the first available partial bound line in the map, scan for linking nonbound lines, until no more linking lines are found.
		// Then, scan the partialBoundMap until the ending partially bound line is found (the B point of this partial line will equal the B point of a linked nonbound line). 
		// When this is done, take all the lines invovled (both nonbound and partial bound) and put them into a new CleaveSequence. Remmove their original copies from the appropriate maps --
		// partial bound lines will be removed from partialBoundMap (and the counter decremented) and the nonbound lines will be removed from the nonboundMap (also decrementing here)
		bool continueSearch = true;
		CategorizedLineSearchResult result = checkForNextNonboundLine(firstPointToSearch);	// search for the first point.
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
				CategorizedLineSearchResult nextResult = checkForNextNonboundLine(nextPointToSearch);
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
		}
		else
		{
			//std::cout << "!! Final partial bound line NOT FOUND! " << std::endl;
			newSequence.sequenceStatus = CleaveSequenceStatus::INCOMPLETE; // mark it as complete

			std::cout << "(CleaveSequenceFactory)  Warning, CleaveSequence is INCOMPLETE. " << std::endl;
			std::cout << "(CleaveSequenceFactory)  Lines are: " << std::endl;
			newSequence.printCategorizedLines();

			int someVal = 3;
			std::cin >> someVal;
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

	}

	while (lineManager.getCountOfIntersectionType(IntersectionType::A_SLICE) > 0)
	{
		//std::cout << "(CleaveSequenceFactory)  STOP! It's hammer time. " << std::endl;
		cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory)  STOP! It's hammer time. ", "\n");

		cleaveSequenceMapRef = in_cleaveMapRef;									// set the map reference that we will export results to.
		auto aslicedMapBegin = lineManager.getBeginIteratorForType(IntersectionType::A_SLICE);					// get the first line in the a_slice map
		int firstLineID = aslicedMapBegin->first;							// store the ID of the first line (for removal later)
		CleaveSequence newSequence;
		insertASliceLineForSequence(&newSequence, firstLineID);
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
		insertASliceSingleInterceptsPointPreciseForSequence(&newSequence, firstLineID);
		int cleaveMapRefSize = int((*in_cleaveMapRef).size());
		(*in_cleaveMapRef)[cleaveMapRefSize] = newSequence;	// insert the sequence.
	}
}

void CleaveSequenceFactory::handleScenarioSingleInterceptsPointPreciseFound(std::map<int, CleaveSequence>* in_cleaveMapRef)
{
	// NEWW
	glm::vec3 lastPointToSearch;
	while (lineManager.getCountOfIntersectionType(IntersectionType::PARTIAL_BOUND) > 0)	// do this until all partial_bound lines have been accounted for. 
	{
		cleaveSequenceMapRef = in_cleaveMapRef;									// set the map reference that we will export results to.

		auto partialBoundMapBegin = lineManager.getBeginIteratorForType(IntersectionType::PARTIAL_BOUND);					// get the first line in the partial bound map
		CategorizedLine* partialBoundLineRef = &partialBoundMapBegin->second;	// get a ref to the line
		int firstLineID = partialBoundMapBegin->first;							// store the ID of the first line (for removal later)
		CleaveSequence newSequence;												// the new line sequence that will eventually be inserted back into the referenced SPoly
		insertFirstPartialBoundLineForSequence(&newSequence, firstLineID);		// insert the first partial bound line we find
		glm::vec3 firstPointToSearch = newSequence.fetchPointToSearch();		// get the searchable point from the first partial bound line we found in the previous step
		//std::cout << "!!! Initial point to search is: " << firstPointToSearch.x << ", " << firstPointToSearch.y << ", " << firstPointToSearch.z << std::endl;


		// first, work with the partially bound lines. Get the first available partial bound line in the map, scan for linking nonbound lines, until no more linking lines are found.
		// Then, scan the partialBoundMap until the ending partially bound line is found (the B point of this partial line will equal the B point of a linked nonbound line). 
		// When this is done, take all the lines invovled (both nonbound and partial bound) and put them into a new CleaveSequence. Remmove their original copies from the appropriate maps --
		// partial bound lines will be removed from partialBoundMap (and the counter decremented) and the nonbound lines will be removed from the nonboundMap (also decrementing here)
		bool continueSearch = true;
		CategorizedLineSearchResult result = checkForNextNonboundLine(firstPointToSearch);	// search for the first point.
		if (result.wasFound == true)		// insert the first categorized line into the sequence, if it was found:
		{
			newSequence.insertNonboundLine(result.returnLine);	// insert the fetched line into the sequence
			bool continueFlag = true;							// check for the next line, at least once.
			while (continueFlag == true)	// loop until this is false.
			{
				glm::vec3 nextPointToSearch = newSequence.fetchPointToSearch();
				CategorizedLineSearchResult nextResult = checkForNextNonboundLine(nextPointToSearch);
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
		//std::cout << "(CleaveSequenceFactory)  !!!!#### NOTICE: there is at least one A_SLICE in the pools...continue? " << std::endl;
		//std::cout << "(CleaveSequenceFactory)  STOP! It's hammer time. " << std::endl;
		cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory)  !!!!#### NOTICE: there is at least one A_SLICE in the pools...continue? ", "\n");
		cleaveSequenceFactoryLogger.log("(CleaveSequenceFactory)  STOP! It's hammer time. ", "\n");

		cleaveSequenceMapRef = in_cleaveMapRef;									// set the map reference that we will export results to.
		auto aslicedMapBegin = lineManager.getBeginIteratorForType(IntersectionType::A_SLICE);					// get the first line in the a_slice map
		int firstLineID = aslicedMapBegin->first;							// store the ID of the first line (for removal later)
		CleaveSequence newSequence;
		insertASliceLineForSequence(&newSequence, firstLineID);
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

}

void CleaveSequenceFactory::handleScenarioMultipleInterceptsPointPrecise(std::map<int, CleaveSequence>* in_cleaveMapRef)
{
	// NEWW
	glm::vec3 lastPointToSearch;
	while (lineManager.getCountOfIntersectionType(IntersectionType::INTERCEPTS_POINT_PRECISE) > 0)
	{
		// get the first intercepts point precise that will be the first line in the CleaveSequence
		cleaveSequenceMapRef = in_cleaveMapRef;
		auto preciseMapBegin = lineManager.getBeginIteratorForType(IntersectionType::INTERCEPTS_POINT_PRECISE);
		int firstLineID = preciseMapBegin->first;							// store the ID of the first line (for removal later)
		CleaveSequence newSequence;
		insertFirstInterceptsPointPreciseForSequence(&newSequence, firstLineID);
		glm::vec3 firstPointToSearch = newSequence.fetchPointToSearch();

		// search for any nonbound lines
		bool continueSearch = true;
		CategorizedLineSearchResult result = checkForNextNonboundLine(firstPointToSearch);	// search for the first point.
		if (result.wasFound == true)		// insert the first categorized line into the sequence, if it was found:
		{
			newSequence.insertNonboundLine(result.returnLine);	// insert the fetched line into the sequence
			bool continueFlag = true;							// check for the next line, at least once.
			while (continueFlag == true)	// loop until this is false.
			{
				glm::vec3 nextPointToSearch = newSequence.fetchPointToSearch();
				CategorizedLineSearchResult nextResult = checkForNextNonboundLine(nextPointToSearch);
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
			//std::cout << "!! Inserting new cleave sequence at index: " << cleaveMapRefSize << std::endl;
			(*in_cleaveMapRef)[cleaveMapRefSize] = newSequence;	// insert the sequence.
			//std::cout << "Map size is now: " << cleaveMapRefSize << std::endl;
		}

		newSequence.printCategorizedLines();

		std::cout << "(CleaveSequenceFactory)  !! End of handle multiple precise loop tick. Continue? " << std::endl;
		int endTest = 3;
		std::cin >> endTest;
	}

}