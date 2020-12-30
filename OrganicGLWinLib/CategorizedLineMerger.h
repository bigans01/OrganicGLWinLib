#pragma once

#ifndef CATEGORIZEDLINEMERGER_H
#define CATEGORIZEDLINEMERGER_H

#include "CategorizedLineGroup.h"
#include "CategorizedLineMergeType.h"
#include "CategorizedLineMergeMachineBase.h"
#include "CategorizedLinePartialBoundMerger.h"
#include "CategorizedLineASliceMerger.h"
#include "CategorizedLinePlaceboMerger.h"
#include "CategorizedLineInterceptsPointPreciseMerger.h"
#include <map>
#include <mutex>

class CleaveSequenceFactory;
class CategorizedLineMerger
{
	public:
		CategorizedLineMerger(CleaveSequenceFactory* in_cleaveSequenceFactoryRef) :
			cleaveSequenceFactoryRef(in_cleaveSequenceFactoryRef)
		{
			buildAndLoadCategorizedLinesIntoMachines();
		};

	private:
		std::map<int, std::unique_ptr<CategorizedLineMergeMachineBase>> machineMap;

		void buildAndLoadCategorizedLinesIntoMachines();
		void runMergingForEachMachine();
		CategorizedLineMergeType determineMergeTypeForGroup(CategorizedLineGroup* in_categorizedLineGroupRef);
		CleaveSequenceFactory* cleaveSequenceFactoryRef = nullptr;

};

#endif
