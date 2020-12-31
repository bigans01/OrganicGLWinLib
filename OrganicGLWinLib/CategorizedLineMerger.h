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
			buildAndLoadCategorizedLinesIntoMachines();		// build the machines, extract the categorized lines into them, and 
															// remove the records from the corresponding CategorizedLineGroups.
															// There should be 0 categorized lines in the CleaveSequenceFactory after this function is done.
			runMergingForEachMachine();
			sendMergedLinesToCleaveSequenceFactory();
		};

	private:
		std::map<int, std::unique_ptr<CategorizedLineMergeMachineBase>> machineMap;

		void buildAndLoadCategorizedLinesIntoMachines();
		void runMergingForEachMachine();
		void sendMergedLinesToCleaveSequenceFactory();
		CategorizedLineMergeType determineMergeTypeForGroup(CategorizedLineGroup* in_categorizedLineGroupRef);
		CleaveSequenceFactory* cleaveSequenceFactoryRef = nullptr;

};

#endif
