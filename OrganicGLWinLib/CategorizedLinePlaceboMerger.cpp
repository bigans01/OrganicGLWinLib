#include "stdafx.h"
#include "CategorizedLinePlaceboMerger.h"
#include "CleaveSequenceFactory.h"

void CategorizedLinePlaceboMerger::runMerging()
{
	std::cout << "Running merge for NO_MERGE_REQUIRED (1 line found). " << std::endl;
	std::cout << "Line counts: " << std::endl;
	std::cout << "PARTIAL_BOUND: " << mergablePartialBoundsCount << std::endl;
	std::cout << "A_SLICE: " << mergableASliceCount << std::endl;
	std::cout << "INTERCEPTS_POINT_PRECISE: " << mergableInterceptsPointPreciseCount << std::endl;
	std::cout << "NON_BOUND: " << mergableNonboundCount << std::endl;

	if (mergablePartialBoundsCount == 1)
	{
		auto targetLineIterator = mergablePartialBoundsMap.begin();
		mergedLineResult = fetchAndRemovePartialBoundMergeCandidate(targetLineIterator->first);
	}
	else if (mergableASliceCount == 1)
	{
		auto targetLineIterator = mergableASliceMap.begin();
		mergedLineResult = fetchAndRemoveASliceMergeCandidate(targetLineIterator->first);
	}
	else if (mergableInterceptsPointPreciseCount == 1)
	{
		auto targetLineIterator = mergableInterceptsPointPreciseMap.begin();
		mergedLineResult = fetchAndRemoveInterceptsPointPreciseMergeCandidate(targetLineIterator->first);
	}
	else if (mergableNonboundCount == 1)
	{
		auto targetLineIterator = mergableNonboundMap.begin();
		mergedLineResult = fetchAndRemoveNonboundMergeCandidate(targetLineIterator->first);
	}

	std::cout << "::::::::::::::::::::::: machine for NO_MERGE_REQUIRED complete. " << std::endl;
	std::cout << "####::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::####" << std::endl;
	std::cout << "####::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::####" << std::endl;
	std::cout << "####::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::####" << std::endl;
	std::cout << "####::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::####" << std::endl;
	int mergeComplete;
	std::cin >> mergeComplete;
}