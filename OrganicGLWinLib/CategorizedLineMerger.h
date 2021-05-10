#pragma once

#ifndef CATEGORIZEDLINEMERGER_H
#define CATEGORIZEDLINEMERGER_H

#include "CategorizedLineGroup.h"
#include "CategorizedLineMergeType.h"
#include "CategorizedLineMergeMachineBase.h"
#include "CategorizedLinePartialBoundMerger.h"
#include "CategorizedLineASliceMerger.h"
#include "CategorizedLinePlaceboMerger.h"
#include "NonBoundMerger.h"
#include "CategorizedLineInterceptsPointPreciseMerger.h"
#include "CategorizedLineInterceptsPreciseToPartialMerger.h"
#include <map>
#include <mutex>
#include "PolyLogger.h"
#include "PolyDebugLevel.h"

// NOTICE: if the CategorizedLineMerger doesn't pull all the lines from the factory and merge them, the engine may not generate an appropriate CleaveSequence.
// Ensure that all categorized lines are extracted from the CleaveSequenceFactory, before merging them and sending them back to it.

class CleaveSequenceFactory;
class CategorizedLineMerger
{
	public:
		CategorizedLineMerger(CleaveSequenceFactory* in_cleaveSequenceFactoryRef, PolyDebugLevel in_polyDebugLevel) :
			cleaveSequenceFactoryRef(in_cleaveSequenceFactoryRef),
			mergerDebugLevel(in_polyDebugLevel)
		{
			mergerLogger.setDebugLevel(in_polyDebugLevel);
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
		PolyLogger mergerLogger;
		PolyDebugLevel mergerDebugLevel = PolyDebugLevel::NONE;

};

#endif
