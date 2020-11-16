#pragma once

#ifndef NextCleaveSequenceFinder_H
#define NextCleaveSequenceFinder_H

#include "SPolyBorderLines.h"
#include "CyclingDirection.h"
#include "BorderLineIntersectRecorder.h"
#include "CleaveSequence.h"
#include "CleaveSequenceMeta.h"
#include "CleaveSequenceCandidateList.h"
#include "CleaveSequenceMetaTracker.h"
#include "DistanceToPoint.h"
#include "PointTranslationCheck.h"
#include "QuatRotationManager.h"
#include "QuatRotationPoints.h"
#include "FoundCleaveSequence.h"
#include "LineWelderRunMode.h"
#include "WeldedLinePool.h"
#include "SelfComparePermit.h"

class NextCleaveSequenceFinder
{
	public:
		NextCleaveSequenceFinder() {};		// just in case
		NextCleaveSequenceFinder(int in_startingborderLineID, 
										SPolyBorderLines* in_borderLineRef, 
										std::map<int, CleaveSequence>* in_cleaveMapRef,
										CyclingDirection in_cyclingDirection,
										CleaveSequenceCandidateList* in_cleaveSequenceCandidateListRef,
										int in_finderStartingCleaveSequenceID,
										CleaveSequenceMetaTracker* in_cleaveSequenceMetaTrackerRef,
										glm::vec3 in_sequenceFinderStartPoint,
										LineWelderRunMode in_lineWelderRunMode,
										WeldedLinePool* in_weldedLinePoolRef,
										SelfComparePermit in_passedPermit) : 
			startingBorderLineID(in_startingborderLineID),
			borderLineRef(in_borderLineRef), 
			cleaveMapRef(in_cleaveMapRef),
			cyclingDirection(in_cyclingDirection),
			cleaveSequenceCandidateListRef(in_cleaveSequenceCandidateListRef),
			finderStartingCleaveSequenceID(in_finderStartingCleaveSequenceID),
			cleaveSequenceMetaTrackerRef(in_cleaveSequenceMetaTrackerRef),
			sequenceFinderStartPoint(in_sequenceFinderStartPoint),
			finderRunMode(in_lineWelderRunMode),
			weldedLinePoolRef(in_weldedLinePoolRef),
			passedPermit(in_passedPermit)
		{
			buildNeighboringCleaveSequenceMap();
		};
		bool wasSequenceFound();
		FoundCleaveSequence getSelectedCleaveSequenceMeta();
	private:
		int startingBorderLineID = 0;				// set upon initialization
		SPolyBorderLines* borderLineRef = nullptr;	// set upon initialization
		std::map<int, CleaveSequence>* cleaveMapRef = nullptr; // set upon initialization
		CyclingDirection cyclingDirection = CyclingDirection::NOVAL;	// set upon initialization, default is NOVAL
		CleaveSequenceCandidateList* cleaveSequenceCandidateListRef = nullptr;		// set upon initialization
		int finderStartingCleaveSequenceID = 0;		// set upon initialization
		CleaveSequenceMetaTracker* cleaveSequenceMetaTrackerRef = nullptr;	// set upon initialization
		glm::vec3 sequenceFinderStartPoint;
		LineWelderRunMode finderRunMode = LineWelderRunMode::NOVAL;	// set upon initialization
		WeldedLinePool* weldedLinePoolRef = nullptr;	// set upon initialization
		SelfComparePermit passedPermit;

		std::set<int> foundSet;						// will contain the other CleaveSequences on this line, except the one that his the value specified by
													// finderStartingCleaveSequenceID.
		bool wasNextSequenceFound = false;				// a flag that determines whether or not neighbors exist.

		FoundCleaveSequence selectedCleaveSequenceMeta;

		void checkForperformingFirstPermitAction();			// designed for PermitType::TYPICAL, it erases the ID of the cleaveSequence in the permit, from the foundSet. This is because a 
															// TYPICAL run should never be compared to its own line on the first run.
		bool checkToPerformSpecialPointFetch();				// special check, for when the permit is for a PermitType::SAME_BORDER_LINE; this allows the CleaveSequence to check
															// itself for the next point, since it begins and ends on the same border line.

		void buildNeighboringCleaveSequenceMap();
		void findAndSortNeighboringCleaveSequences();
};

#endif
