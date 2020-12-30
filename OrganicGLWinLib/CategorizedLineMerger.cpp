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
	std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
	int categorizedLineMergerStop = 3;
	std::cin >> categorizedLineMergerStop;

}

CategorizedLineMergeType CategorizedLineMerger::determineMergeTypeForGroup(CategorizedLineGroup* in_categorizedLineGroupRef)
{
	CategorizedLineMergeType returnType = CategorizedLineMergeType::NOVAL;

	return returnType;
}

void CategorizedLineMerger::runMergingForEachMachine()
{

}
