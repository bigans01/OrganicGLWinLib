#pragma once

#ifndef NEIGHBORINGCLEAVESEQUENCEFINDER_H
#define NEIGHBORINGCLEAVESEQUENCEFINDER_H

#include "SPolyBorderLines.h"
#include "CyclingDirection.h"
#include "BorderLineIntersectRecorder.h"

class NeighboringCleaveSequenceFinder
{
	public:
		NeighboringCleaveSequenceFinder() {};		// just in case
		NeighboringCleaveSequenceFinder(int in_startingborderLineID, SPolyBorderLines* in_borderLineRef, CyclingDirection in_cyclingDirection) : 
			startingBorderLineID(in_startingborderLineID),
			borderLineRef(in_borderLineRef), 
			cyclingDirection(in_cyclingDirection) 
		{
			buildNeighboringCleaveSequenceMap();
		};
		bool wereNeighborsFound();
	private:
		int startingBorderLineID = 0;				// set upon initialization
		SPolyBorderLines* borderLineRef = nullptr;	// set upon initialization
		CyclingDirection cyclingDirection = CyclingDirection::NOVAL;	// set upon initialization, default is NOVAL
		bool doNeighborsExist = false;				// a flag that determines whether or not neighbors exist.
		void buildNeighboringCleaveSequenceMap();
		void findAndSortNeighboringCleaveSequences();
};

#endif
