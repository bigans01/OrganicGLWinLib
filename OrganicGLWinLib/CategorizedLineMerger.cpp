#include "stdafx.h"
#include "CategorizedLineMerger.h"
#include "CleaveSequenceFactory.h"


void CategorizedLineMerger::buildAndLoadCategorizedLinesIntoMachines()
{
	// cycle through the CleaveSequenceFactory's CategorizedLineGroupMap, to generate the appropriate machine for each group, and then extract
	// the categorized lines for each machine.
	auto factoryGroupMapBegin = cleaveSequenceFactoryRef->groupMap.groups.begin();
	auto factoryGroupMapEnd = cleaveSequenceFactoryRef->groupMap.groups.end();
	for (; factoryGroupMapBegin != factoryGroupMapEnd; factoryGroupMapBegin++)
	{
		std::cout << "!! Determining machine type, for guest SPoly with ID " << factoryGroupMapBegin->first << std::endl;
		CategorizedLineMergeType currentGroupMergeType = determineMergeTypeForGroup(&factoryGroupMapBegin->second);
		if (currentGroupMergeType == CategorizedLineMergeType::MERGE_TO_PARTIAL_BOUND)
		{
			machineMap[factoryGroupMapBegin->first].reset(new CategorizedLinePartialBoundMerger);
			machineMap[factoryGroupMapBegin->first]->initialize(cleaveSequenceFactoryRef, &factoryGroupMapBegin->second);
			machineMap[factoryGroupMapBegin->first]->extractCategorizedLines();
		}
		else if (currentGroupMergeType == CategorizedLineMergeType::MERGE_TO_INTERCEPTS_POINT_PRECISE)
		{
			machineMap[factoryGroupMapBegin->first].reset(new CategorizedLineInterceptsPointPreciseMerger);
			machineMap[factoryGroupMapBegin->first]->initialize(cleaveSequenceFactoryRef, &factoryGroupMapBegin->second);
			machineMap[factoryGroupMapBegin->first]->extractCategorizedLines();
		}

		else if (currentGroupMergeType == CategorizedLineMergeType::MERGE_TO_A_SLICE)
		{
			machineMap[factoryGroupMapBegin->first].reset(new CategorizedLineASliceMerger);
			machineMap[factoryGroupMapBegin->first]->initialize(cleaveSequenceFactoryRef, &factoryGroupMapBegin->second);
			machineMap[factoryGroupMapBegin->first]->extractCategorizedLines();
		}
		else if (currentGroupMergeType == CategorizedLineMergeType::NO_MERGE_REQUIRED)
		{
			machineMap[factoryGroupMapBegin->first].reset(new CategorizedLinePlaceboMerger);
			machineMap[factoryGroupMapBegin->first]->initialize(cleaveSequenceFactoryRef, &factoryGroupMapBegin->second);
			machineMap[factoryGroupMapBegin->first]->extractCategorizedLines();
		}
	}

	std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
	std::cout << "::::::::::::::::::::::: Notice: CategorizedLineMerger stop. " << std::endl;
	cleaveSequenceFactoryRef->printLineCounts();
	std::cout << ":::::: Size of CleaveSequenceFactory's group map: " << cleaveSequenceFactoryRef->groupMap.groups.size() << std::endl;
	std::cout << ":::::: Printing line counts in group map: " <<  std::endl;
	cleaveSequenceFactoryRef->groupMap.printGroupLineCounts();
	std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
	int categorizedLineMergerStop = 3;
	std::cin >> categorizedLineMergerStop;

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

	// determine the appropriate machine type, based off the counts.

	// CASE 1: PARTIAL_BOUND machine
	if
	(
		(partialBoundCount == 1)
		&&
		(interceptsPointPreciseCount == 0)
		&&
		(aSliceCount == 0)
	)
	{
		returnType = CategorizedLineMergeType::MERGE_TO_PARTIAL_BOUND;
		std::cout << "! Result is MERGE_TO_PARTIAL_BOUND. " << std::endl;
	}

	// CASE 2: A_SLICE machine
	else if
	(
		(partialBoundCount == 2)
		||
		(aSliceCount == 1)
	)
	{
		returnType = CategorizedLineMergeType::MERGE_TO_A_SLICE;
		std::cout << "! Result is MERGE_TO_A_SLICE. " << std::endl;
	}

	// CASE 3: INTERCEPTS_POINT_PRECISE machine
	else if
	(
		(interceptsPointPreciseCount == 1)
	)
	{
		returnType = CategorizedLineMergeType::MERGE_TO_INTERCEPTS_POINT_PRECISE;
		std::cout << "! Result is INTERCEPTS_POINT_PRECISE. " << std::endl;
	}

	return returnType;
}

void CategorizedLineMerger::runMergingForEachMachine()
{

}
