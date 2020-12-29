#pragma once

#ifndef CATEGORIZEDLINEMERGER_H
#define CATEGORIZEDLINEMERGER_H

#include "CategorizedLineGroup.h"
#include "CategorizedLineMergeType.h"
#include "CategorizedLineMergeMachineBase.h"
#include "CategorizedLinePartialBoundMerger.h"
#include "CategorizedLineASliceMerger.h"
#include "CategorizedLinePlaceboMerger.h"
#include <map>
#include <mutex>

class CleaveSequenceFactory;
class CategorizedLineMerger
{
	public:
		CategorizedLineMerger(CleaveSequenceFactory* in_cleaveSequenceFactoryRef) :
			cleaveSequenceFactoryRef(in_cleaveSequenceFactoryRef)
		{
			runMerging();
		};

	private:
		//std::map<int, CategorizedLineMergeMachineBase> machineMap;
		std::map<int, std::unique_ptr<CategorizedLineMergeMachineBase>> machineMap;

		void runMerging();
		CategorizedLineMergeType determineMergeTypeForGroup(CategorizedLineGroup* in_categorizedLineGroupRef);
		CleaveSequenceFactory* cleaveSequenceFactoryRef = nullptr;

};

#endif
