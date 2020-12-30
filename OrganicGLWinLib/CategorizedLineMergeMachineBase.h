#pragma once

#ifndef CATEGORIZEDLINEMERGEMACHINEBASE_H
#define CATEGORIZEDLINEMERGEMACHINEBASE_H

#include "CategorizedLine.h"
#include "CategorizedLineGroup.h"
//#include "CleaveSequenceFactory.h"
#include <map>

class CleaveSequenceFactory;
class CategorizedLineMergeMachineBase
{
	public:
		void initialize(CleaveSequenceFactory* in_cleaveSequenceFactoryRef, CategorizedLineGroup* in_categorizedLineGroupRef);
		void extractCategorizedLines();
		virtual void runMerging() = 0;		// defined by children;
		CategorizedLine fetchProducedLine();

		CleaveSequenceFactory* cleaveSequenceFactoryRef = nullptr;
		CategorizedLineGroup* categorizedLineGroupRef = nullptr;

		std::map<int, CategorizedLine> mergablePartialBoundsMap;
		std::map<int, CategorizedLine> mergableNonboundMap;
		std::map<int, CategorizedLine> mergableInterceptsPointPreciseMap;
		std::map<int, CategorizedLine> mergableASliceMap;
		int mergablePartialBoundsIndex = 0;
		int mergableNonboundIndex = 0;
		int mergableInterceptsPointPreciseIndex = 0;
		int mergableASliceIndex = 0;

		CategorizedLine mergedLineResult;	// the final product of the merging


};

#endif
