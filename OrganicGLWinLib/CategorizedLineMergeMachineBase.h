#pragma once

#ifndef CATEGORIZEDLINEMERGEMACHINEBASE_H
#define CATEGORIZEDLINEMERGEMACHINEBASE_H

#include "CategorizedLine.h"
#include "CategorizedLineGroup.h"
#include "CategorizedLineGroupLocation.h"
#include "CategorizedLineSearchResult.h"
#include "PolyLogger.h"
#include "PolyDebugLevel.h"

class CleaveSequenceFactory;
class CategorizedLineMergeMachineBase
{
	public:
		void initialize(CleaveSequenceFactory* in_cleaveSequenceFactoryRef, CategorizedLineGroup* in_categorizedLineGroupRef, PolyDebugLevel in_polyDebugLevel);
		void extractCategorizedLines();
		virtual void runMerging() = 0;		// defined by children;
		CategorizedLine fetchProducedLine();

		CleaveSequenceFactory* cleaveSequenceFactoryRef = nullptr;
		CategorizedLineGroup* categorizedLineGroupRef = nullptr;

		std::map<int, CategorizedLine> mergablePartialBoundsMap;
		std::map<int, CategorizedLine> mergableNonboundMap;
		std::map<int, CategorizedLine> mergableInterceptsPointPreciseMap;
		std::map<int, CategorizedLine> mergableASliceMap;
		std::map<int, CategorizedLine> mergableASliceSingleInterceptsPointPreciseMap;
		int mergablePartialBoundsCount = 0;
		int mergableNonboundCount = 0;
		int mergableInterceptsPointPreciseCount = 0;
		int mergableASliceCount = 0;
		int mergableASliceSingleInterceptsPointPreciseCount = 0;

		CategorizedLine fetchAndRemovePartialBoundMergeCandidate(int in_fetchIndex);
		CategorizedLine fetchAndRemoveASliceMergeCandidate(int in_fetchIndex);
		CategorizedLine fetchAndRemoveASliceSingleInterceptsPointPreciseCandidate(int in_fetchIndex);
		CategorizedLine fetchAndRemoveNonboundMergeCandidate(int in_fetchIndex);
		CategorizedLine fetchAndRemoveInterceptsPointPreciseMergeCandidate(int in_fetchIndex);
		CategorizedLineSearchResult checkForNextNonboundMergeCandidate(glm::vec3 in_pointToSearch);
		CategorizedLineSearchResult getClosestNonBoundMergeCandidate(glm::vec3 in_pointToSearch); // created for MERGE-0, 3/24/2021.

		CategorizedLine mergedLineResult;	// the final product of the merging

		PolyLogger mergeMachineLogger;
};

#endif
