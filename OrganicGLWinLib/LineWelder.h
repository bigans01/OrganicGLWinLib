#pragma once

#ifndef LINEWELDER_H
#define LINEWELDER_H

#include "SPoly.h"
#include "BorderLineIDPair.h"
#include "MassManipulationMode.h"
#include "BorderLineCycler.h"
#include "NeighboringCleaveSequenceFinder.h"
#include "CleaveSequenceCandidateListMap.h"
#include "CleaveSequenceMetaTracker.h"

class LineWelder
{
public:
	LineWelder(SPoly* in_sPolyRef) : sPolyRef(in_sPolyRef) 
	{
		currentManipulationMode = sPolyRef->massManipulationSetting;	// set the manipulation mode.
		startWelding();
	};
private:
	SPoly* sPolyRef = nullptr;	// a reference to the SPoly we will be operating on for this LineWelder.
	MassManipulationMode currentManipulationMode = MassManipulationMode::CREATION;	// CREATION is default value, but will be overrriden by the SPoly's value when
																					// the LineWelder is constructed.
	CyclingDirection foundDirection;	// the direction that the LineWelder is going in (FORWARD) = towards point B of border lines, (REVERSE) = towards point A of border lines.
	CleaveSequenceCandidateListMap candidateListMap;
	CleaveSequenceMetaTracker metaTracker;
	int currentBorderLineID = 0;	// the border line we'll be starting on.
	int endingBorderLineID = 0;		// the border line we'll be ending on (the while loop will end when it hits this)
	void getCleaveSequenceCandidateListMap();	// calls the sPolyRef to return a built CleaveSequenceCandidateListMap
	void getCleaveSequenceMetaTracker();		// calls the sPolyRef to return a built CleaveSequenceMetaTracker
	void startWelding();
	void findRemainingWeldingLines(int in_currentBorderLineID, glm::vec3 in_leadingPoint, CleaveSequenceCandidateList* in_cleaveSequenceCandidateListRef, int in_finderStartingCleaveSequenceID);
	void findWeldingLines(int in_startingBorderLineID, int in_startingCleaveSequenceID, int in_categorizedLineInCleaveSequenceID, CyclingDirection in_cyclingDirection, BorderLinePointPair in_beginningPointPair);
};

#endif
