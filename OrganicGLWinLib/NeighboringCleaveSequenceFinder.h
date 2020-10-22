#pragma once

#ifndef NEIGHBORINGCLEAVESEQUENCEFINDER_H
#define NEIGHBORINGCLEAVESEQUENCEFINDER_H

#include "SPolyBorderLines.h"
#include "CyclingDirection.h"
#include "BorderLineIntersectRecorder.h"
#include "CleaveSequence.h"
#include "CleaveSequenceMeta.h"
#include "CleaveSequenceCandidateList.h"
#include "CleaveSequenceMetaTracker.h"

class NeighboringCleaveSequenceFinder
{
	public:
		NeighboringCleaveSequenceFinder() {};		// just in case
		NeighboringCleaveSequenceFinder(int in_startingborderLineID, 
										SPolyBorderLines* in_borderLineRef, 
										std::map<int, CleaveSequence>* in_cleaveMapRef,
										CyclingDirection in_cyclingDirection,
										CleaveSequenceCandidateList* in_cleaveSequenceCandidateListRef,
										int in_finderStartingCleaveSequenceID,
										CleaveSequenceMetaTracker* in_cleaveSequenceMetaTrackerRef) : 
			startingBorderLineID(in_startingborderLineID),
			borderLineRef(in_borderLineRef), 
			cleaveMapRef(in_cleaveMapRef),
			cyclingDirection(in_cyclingDirection),
			cleaveSequenceCandidateListRef(in_cleaveSequenceCandidateListRef),
			finderStartingCleaveSequenceID(in_finderStartingCleaveSequenceID),
			cleaveSequenceMetaTrackerRef(in_cleaveSequenceMetaTrackerRef)
		{
			buildNeighboringCleaveSequenceMap();
		};
		bool wereNeighborsFound();
	private:
		int startingBorderLineID = 0;				// set upon initialization
		SPolyBorderLines* borderLineRef = nullptr;	// set upon initialization
		std::map<int, CleaveSequence>* cleaveMapRef = nullptr; // set upon initialization
		CyclingDirection cyclingDirection = CyclingDirection::NOVAL;	// set upon initialization, default is NOVAL
		CleaveSequenceCandidateList* cleaveSequenceCandidateListRef = nullptr;		// set upon initialization
		CleaveSequenceMetaTracker* cleaveSequenceMetaTrackerRef = nullptr;	// set upon initialization
		int finderStartingCleaveSequenceID = 0;		// set upon initialization

		std::set<int> foundSet;						// will contain the other CleaveSequences on this line, except the one that his the value specified by
													// finderStartingCleaveSequenceID.
		bool doNeighborsExist = false;				// a flag that determines whether or not neighbors exist.
		void buildNeighboringCleaveSequenceMap();
		void findAndSortNeighboringCleaveSequences();
};

#endif
