#include "stdafx.h"
#include "CategorizedLineMerger.h"
#include "CleaveSequenceFactory.h"


void CategorizedLineMerger::buildAndLoadCategorizedLinesIntoMachines()
{
	// print out the counts of intercepts in each border line of the SPoly
	//auto sPolyBordeLinesBegin = cleaveSequenceFactoryRef->sPolyRef


	// cycle through the CleaveSequenceFactory's CategorizedLineGroupMap, to generate the appropriate machine for each group, and then extract
	// the categorized lines for each machine.
	auto factoryGroupMapBegin = cleaveSequenceFactoryRef->groupMap.groups.begin();
	auto factoryGroupMapEnd = cleaveSequenceFactoryRef->groupMap.groups.end();
	for (; factoryGroupMapBegin != factoryGroupMapEnd; factoryGroupMapBegin++)
	{
		//std::cout << "!! Determining machine type, for guest SPoly with ID " << factoryGroupMapBegin->first << std::endl;
		mergerLogger.log("(CategorizedLineMerger) !! Determining machine type, for guest SPoly with ID ", factoryGroupMapBegin->first, "\n");
		CategorizedLineMergeType currentGroupMergeType = determineMergeTypeForGroup(&factoryGroupMapBegin->second);
		if (currentGroupMergeType == CategorizedLineMergeType::MERGE_TO_PARTIAL_BOUND)
		{
			machineMap[factoryGroupMapBegin->first].reset(new CategorizedLinePartialBoundMerger);
			machineMap[factoryGroupMapBegin->first]->initialize(cleaveSequenceFactoryRef, &factoryGroupMapBegin->second, mergerDebugLevel);
			machineMap[factoryGroupMapBegin->first]->extractCategorizedLines();
		}
		else if (currentGroupMergeType == CategorizedLineMergeType::MERGE_TO_INTERCEPTS_POINT_PRECISE)
		{
			machineMap[factoryGroupMapBegin->first].reset(new CategorizedLineInterceptsPointPreciseMerger);
			machineMap[factoryGroupMapBegin->first]->initialize(cleaveSequenceFactoryRef, &factoryGroupMapBegin->second, mergerDebugLevel);
			machineMap[factoryGroupMapBegin->first]->extractCategorizedLines();
		}

		else if (currentGroupMergeType == CategorizedLineMergeType::MERGE_TO_A_SLICE)
		{
			machineMap[factoryGroupMapBegin->first].reset(new CategorizedLineASliceMerger);
			machineMap[factoryGroupMapBegin->first]->initialize(cleaveSequenceFactoryRef, &factoryGroupMapBegin->second, mergerDebugLevel);
			machineMap[factoryGroupMapBegin->first]->extractCategorizedLines();
		}
		else if (currentGroupMergeType == CategorizedLineMergeType::NO_MERGE_REQUIRED)
		{
			machineMap[factoryGroupMapBegin->first].reset(new CategorizedLinePlaceboMerger);
			machineMap[factoryGroupMapBegin->first]->initialize(cleaveSequenceFactoryRef, &factoryGroupMapBegin->second, mergerDebugLevel);
			machineMap[factoryGroupMapBegin->first]->extractCategorizedLines();
		}
	}

	//std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
	//std::cout << "::::::::::::::::::::::: Notice: CategorizedLineMerger stop. " << std::endl;
	mergerLogger.log("(CategorizedLineMerger) !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!", "\n");
	mergerLogger.log("(CategorizedLineMerger) ::::::::::::::::::::::: Notice: CategorizedLineMerger stop. ", "\n");
	if (mergerLogger.isLoggingSet() == true)
	{
		mergerLogger.log("(CategorizedLineMerger) >>>> starting printing line data as it exists in the referenced CleaveSequenceFactory...", "\n");
		cleaveSequenceFactoryRef->printLineCounts();
		mergerLogger.log("(CategorizedLineMerger) >>>> finished printing line data as it exists in the referenced CleaveSequenceFactory...", "\n");
	}
	//std::cout << ":::::: Size of CleaveSequenceFactory's group map: " << cleaveSequenceFactoryRef->groupMap.groups.size() << std::endl;
	//std::cout << ":::::: Printing line counts in group map: " <<  std::endl;
	mergerLogger.log("(CategorizedLineMerger) :::::: Size of CleaveSequenceFactory's group map: ", cleaveSequenceFactoryRef->groupMap.groups.size(), "\n");
	mergerLogger.log("(CategorizedLineMerger) :::::: Printing line counts in group map: ", "\n");

	if (mergerLogger.isLoggingSet() == true)
	{
		mergerLogger.log("(CategorizedLineMerger) >>>> starting printing group line counts in referenced CleaveSequenceFactory...", "\n");
		cleaveSequenceFactoryRef->groupMap.printGroupLineCounts();
		mergerLogger.log("(CategorizedLineMerger) >>>> finished printing group line counts in referenced CleaveSequenceFactory...", "\n");
	}
	//std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
	mergerLogger.log("(CategorizedLineMerger) !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!", "\n");
	mergerLogger.waitForDebugInput();

	//int categorizedLineMergerStop = 3;
	//std::cin >> categorizedLineMergerStop;

}

CategorizedLineMergeType CategorizedLineMerger::determineMergeTypeForGroup(CategorizedLineGroup* in_categorizedLineGroupRef)
{
	CategorizedLineMergeType returnType = CategorizedLineMergeType::NOVAL;

	// determine the counts for each line type.
	int partialBoundCount = 0;
	int interceptsPointPreciseCount = 0;
	int aSliceCount = 0;
	int nonBoundCount = 0;
	auto categorizedLineGroupRecordsBegin = in_categorizedLineGroupRef->recordVector.begin();
	auto categorizedLineGroupRecordsEnd = in_categorizedLineGroupRef->recordVector.end();
	for (; categorizedLineGroupRecordsBegin != categorizedLineGroupRecordsEnd; categorizedLineGroupRecordsBegin++)
	{
		if (categorizedLineGroupRecordsBegin->categorizedLineIntersectionType == IntersectionType::PARTIAL_BOUND)
		{
			partialBoundCount++;
		}
		else if (categorizedLineGroupRecordsBegin->categorizedLineIntersectionType == IntersectionType::INTERCEPTS_POINT_PRECISE)
		{
			interceptsPointPreciseCount++;
		}
		else if (categorizedLineGroupRecordsBegin->categorizedLineIntersectionType == IntersectionType::A_SLICE)
		{
			aSliceCount++;
		}
		else if (categorizedLineGroupRecordsBegin->categorizedLineIntersectionType == IntersectionType::NON_BOUND)
		{
			nonBoundCount++;
		}
	}

	int totalLineCount = partialBoundCount + interceptsPointPreciseCount + aSliceCount + nonBoundCount;

	// CASE 1: NO_MERGE_REQUIRED machine.
	if 
	(
		(totalLineCount == 1)
	)
	{
		returnType = CategorizedLineMergeType::NO_MERGE_REQUIRED;
		//std::cout << "! Result is NO_MERGE_REQUIRED. " << std::endl;
		mergerLogger.log("(CategorizedLineMerger) ! Result is NO_MERGE_REQUIRED. ", "\n");
	}
	// determine the appropriate machine type, based off the counts.

	// CASE 2: PARTIAL_BOUND machine
	else if
	(
		(partialBoundCount == 1)
		&&
		(interceptsPointPreciseCount == 0)
		&&
		(aSliceCount == 0)
	)
	{
		returnType = CategorizedLineMergeType::MERGE_TO_PARTIAL_BOUND;
		//std::cout << "! Result is MERGE_TO_PARTIAL_BOUND. " << std::endl;
		mergerLogger.log("(CategorizedLineMerger) ! Result is MERGE_TO_PARTIAL_BOUND. ", "\n");
	}

	// CASE 3: A_SLICE machine
	else if
	(
		(partialBoundCount == 2)
		||
		(aSliceCount == 1)
	)
	{
		returnType = CategorizedLineMergeType::MERGE_TO_A_SLICE;
		//std::cout << "! Result is MERGE_TO_A_SLICE. " << std::endl;
		mergerLogger.log("(CategorizedLineMerger) ! Result is MERGE_TO_A_SLICE. ", "\n");
	}

	// CASE 4: INTERCEPTS_POINT_PRECISE machine
	else if
	(
		(interceptsPointPreciseCount == 1)
	)
	{
		returnType = CategorizedLineMergeType::MERGE_TO_INTERCEPTS_POINT_PRECISE;
		//std::cout << "! Result is INTERCEPTS_POINT_PRECISE. " << std::endl;
		mergerLogger.log("(CategorizedLineMerger) ! Result is INTERCEPTS_POINT_PRECISE. ", "\n");
	}

	return returnType;
}

void CategorizedLineMerger::runMergingForEachMachine()
{
	auto machineMapBegin = machineMap.begin();
	auto machineMapEnd = machineMap.end();
	for (; machineMapBegin != machineMapEnd; machineMapBegin++)
	{
		machineMapBegin->second->runMerging();
	}
}

void CategorizedLineMerger::sendMergedLinesToCleaveSequenceFactory()
{
	auto machineMapBegin = machineMap.begin();
	auto machineMapEnd = machineMap.end();
	for (; machineMapBegin != machineMapEnd; machineMapBegin++)
	{
		CategorizedLine currentLine = machineMapBegin->second->fetchProducedLine();
		int superDumbVal = 3;

		//std::cout << "!! Current Line stats: " << std::endl;
		//std::cout << "point A: " << currentLine.line.pointA.x << ", " << currentLine.line.pointA.y << ", " << currentLine.line.pointA.z << std::endl;
		//std::cout << "point B: " << currentLine.line.pointB.x << ", " << currentLine.line.pointB.y << ", " << currentLine.line.pointB.z << std::endl;
		//std::cout << "Empty normal: " << currentLine.emptyNormal.x << ", " << currentLine.emptyNormal.y << ", " << currentLine.emptyNormal.z << std::endl;

		mergerLogger.log("(CategorizedLineMerger) !! Current Line stats: ", "\n");
		mergerLogger.log("(CategorizedLineMerger) point A: ", currentLine.line.pointA.x, ", ", currentLine.line.pointA.y, ", ", currentLine.line.pointA.z, "\n");
		mergerLogger.log("(CategorizedLineMerger) point B: ", currentLine.line.pointB.x, ", ", currentLine.line.pointB.y, ", ", currentLine.line.pointB.z, "\n");
		mergerLogger.log("(CategorizedLineMerger) Empty normal: ", currentLine.emptyNormal.x, ", ", currentLine.emptyNormal.y, ", ", currentLine.emptyNormal.z, "\n");

		// put the line back into the factory, via the appropriate functon call:
		if (currentLine.type == IntersectionType::PARTIAL_BOUND)
		{
			//std::cout << ">> Inserting this PARTIAL_BOUND line back into the CleaveSequenceFactory." << std::endl;
			mergerLogger.log("(CategorizedLineMerger) >> Inserting this PARTIAL_BOUND line back into the CleaveSequenceFactory.", "\n");
			cleaveSequenceFactoryRef->insertPartialBoundLine(currentLine);
		}
		else if (currentLine.type == IntersectionType::NON_BOUND)
		{
			//std::cout << ">> Inserting this NON_BOUND line back into the CleaveSequenceFactory." << std::endl;
			mergerLogger.log("(CategorizedLineMerger) >> Inserting this NON_BOUND line back into the CleaveSequenceFactory.", "\n");
			cleaveSequenceFactoryRef->insertNonboundLine(currentLine);
		}
		else if (currentLine.type == IntersectionType::INTERCEPTS_POINT_PRECISE)
		{
			//std::cout << ">> Inserting this INTERCEPTS_POINT_PRECISE line back into the CleaveSequenceFactory." << std::endl;
			mergerLogger.log("(CategorizedLineMerger) >> Inserting this INTERCEPTS_POINT_PRECISE line back into the CleaveSequenceFactory.", "\n");
			cleaveSequenceFactoryRef->insertInterceptsPointPrecise(currentLine);
		}
		else if (currentLine.type == IntersectionType::A_SLICE)
		{
			//std::cout << ">> Inserting this A_SLICE line back into the CleaveSequenceFactory." << std::endl;
			mergerLogger.log("(CategorizedLineMerger) >> Inserting this A_SLICE line back into the CleaveSequenceFactory.", "\n");
			cleaveSequenceFactoryRef->insertAslicedLine(currentLine);
		}

		//currentLine.testFunction();
	}

	
	
	//std::cout << "!!! Finished call of sendMergedLinesToCleaveSequenceFactory. " << std::endl;
	//int finishVal = 3;
	//std::cin >> finishVal;

	mergerLogger.log("(CategorizedLineMerger) !!! Finished call of sendMergedLinesToCleaveSequenceFactory. ", "\n");
	mergerLogger.waitForDebugInput();
}
