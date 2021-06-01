#pragma once

#ifndef LINEWELDER_H
#define LINEWELDER_H

#include "SPoly.h"
#include "BorderLineIDPair.h"
#include "MassManipulationMode.h"
#include "BorderLineCycler.h"
#include "NextCleaveSequenceFinder.h"
#include "CleaveSequenceCandidateListMap.h"
#include "CleaveSequenceMetaTracker.h"
#include "CleaveSequence.h"
#include "FoundCleaveSequence.h"
#include "LineWelderHierarchyPosition.h"
#include "LineWelderRunMode.h"
#include "WeldedLinePool.h"
#include "SelfComparePermit.h"
#include "PolyLogger.h"
#include "PolyDebugLevel.h"
#include "UsableCleaveSequenceCalculator.h"
#include "OperableIntSet.h"
#include "ConsumedCleaveSequenceGroups.h"

class LineWelder
{
public:
	LineWelder(SPoly* in_sPolyRef, PolyDebugLevel in_polyDebugLevel) : 
		sPolyRef(in_sPolyRef),
		lineWelderLoggerDebugLevel(in_polyDebugLevel)
	{
		lineWelderLogger.setDebugLevel(in_polyDebugLevel);
		currentManipulationMode = sPolyRef->massManipulationSetting;	// set the manipulation mode.
		getCleaveSequenceCandidateListMap();	// create the candidates map for each border line, by calling on the SPoly to do it
		getCleaveSequenceMetaTracker();			// builds the meta tracker, so that we may pass it to the NextCleaveSequenceFinder
	};
	void startWelding();
	void purgeNegligibleLinesInPool();
	int getRemainingCandidateCount();
	WeldedLinePool retrieveLinePool();
	void clearLinePool();
	void printConsumedSequenceGroups();
private:
	SPoly* sPolyRef = nullptr;	// a reference to the SPoly we will be operating on for this LineWelder.
	MassManipulationMode currentManipulationMode = MassManipulationMode::CREATION;	// CREATION is default value, but will be overrriden by the SPoly's value when
																					// the LineWelder is constructed.
	LineWelderHierarchyPosition currentHierarchyPositionOfLatestCleaveSequence = LineWelderHierarchyPosition::NOVAL;	// always starts as NOVAL, until a CleaveSequence is found -- at that point
																														// this always changes to either SUB or SUPER.
	CyclingDirection foundDirection;	// the direction that the LineWelder is going in (FORWARD) = towards point B of border lines, (REVERSE) = towards point A of border lines.
	CleaveSequenceCandidateListMap candidateListMap;
	glm::vec3 currentLeadingPoint;		
	CleaveSequenceMetaTracker metaTracker;
	SelfComparePermit permit;

	LineWelderRunMode runMode = LineWelderRunMode::NOVAL;	// begins as NOVAL, but should start as CONTINUE, and end as ENDING (when we are on the last border line of the welder)
	WeldedLinePool weldedLines;

	int beginningSequenceID = 0;	// the sequence ID where the welder will start, and begin.
	int currentBorderLineID = 0;	// the border line we'll be starting on.
	int endingBorderLineID = 0;		// the border line we'll be ending on (the while loop will end when it hits this)
	void getCleaveSequenceCandidateListMap();	// calls the sPolyRef to return a built CleaveSequenceCandidateListMap
	void getCleaveSequenceMetaTracker();		// calls the sPolyRef to return a built CleaveSequenceMetaTracker
	void findRemainingWeldingLines(int in_currentBorderLineID, 
									glm::vec3 in_leadingPoint, 
									CleaveSequenceCandidateList* in_cleaveSequenceCandidateListRef, 
									int in_finderStartingCleaveSequenceID,
									OperableIntSet in_unusableSet);
	void insertNewWeldingLine(glm::vec3 in_pointA, glm::vec3 in_pointB, glm::vec3 in_emptyNormal, bool in_wasLineInsertedAslice);
	void updateLeadingPointAndInsertNewWeldingLineFromBorderLineData();

	PolyLogger lineWelderLogger;
	PolyDebugLevel lineWelderLoggerDebugLevel = PolyDebugLevel::NONE;
	int runIteration = 0;			// the ID of the run we are on; used to insert into ConsumedCleaveSequenceGroups
	ConsumedCleaveSequenceGroups consumedSequenceGroups;
	bool wasLastWeldingLineInsertedASlice = false;
};

#endif
