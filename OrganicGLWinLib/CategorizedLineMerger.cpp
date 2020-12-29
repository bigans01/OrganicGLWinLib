#include "stdafx.h"
#include "CategorizedLineMerger.h"
#include "CleaveSequenceFactory.h"

void CategorizedLineMerger::runMerging()
{
	// cycle through the CleaveSequenceFactory's CategorizedLineGroupMap
	auto factoryGroupMapBegin = cleaveSequenceFactoryRef->groupMap.groups.begin();
	auto factoryGroupMapEnd = cleaveSequenceFactoryRef->groupMap.groups.end();
	for (; factoryGroupMapBegin != factoryGroupMapEnd; factoryGroupMapBegin++)
	{
		CategorizedLineMergeType currentGroupMergeType = determineMergeTypeForGroup(&factoryGroupMapBegin->second);
		if (currentGroupMergeType == CategorizedLineMergeType::MERGE_TO_PARTIAL_BOUND)
		{
			machineMap[factoryGroupMapBegin->first].reset(new CategorizedLinePartialBoundMerger);
			machineMap[factoryGroupMapBegin->first]->initialize(cleaveSequenceFactoryRef, &factoryGroupMapBegin->second);
		}
		else if (currentGroupMergeType == CategorizedLineMergeType::MERGE_TO_A_SLICE)
		{
			machineMap[factoryGroupMapBegin->first].reset(new CategorizedLineASliceMerger);
			machineMap[factoryGroupMapBegin->first]->initialize(cleaveSequenceFactoryRef, &factoryGroupMapBegin->second);
		}
		else if (currentGroupMergeType == CategorizedLineMergeType::NO_MERGE_REQUIRED)
		{
			machineMap[factoryGroupMapBegin->first].reset(new CategorizedLinePlaceboMerger);
			machineMap[factoryGroupMapBegin->first]->initialize(cleaveSequenceFactoryRef, &factoryGroupMapBegin->second);
		}
	}
}

CategorizedLineMergeType CategorizedLineMerger::determineMergeTypeForGroup(CategorizedLineGroup* in_categorizedLineGroupRef)
{
	CategorizedLineMergeType returnType = CategorizedLineMergeType::NOVAL;
	return returnType;
}

