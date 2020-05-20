#pragma once

#ifndef BORDERLINECYCLE_H
#define BORDERLINECYCLE_H

#include "SPoly.h"
#include "SPolyBorderLines.h"
#include "CyclingDirection.h"

class BorderLineCycle
{
	public:
		void initialize(SPoly* in_sPolyRef, int in_borderLineStartIndex, int in_borderLineEndIndex, CyclingDirection in_direction);
		SPolyBorderLines cycle[8];	// a single cycle will have up to 8 border lines (probably not this many ever, but ok)
		SPoly* sPolyRef;
		short startIndex = 0;				// the index of the first border line to run
		short endIndex = 0;					// the index of the ending border line (from the last categorized line in a cleave sequence)
		short numberOfBorderLines = 0;		// the number of border lines
		short borderLineFinalIndex = 0;		// the index of the final border line (if there are 4 total border lines, this value is 3)
		CyclingDirection direction = CyclingDirection::NOVAL;		// the calculated direction
		short cycleCount = 0;			// the number of lines in the cycle (up to 8)
		short firstIterationSetNumberOfIterations = 0;	// how many times the first set must iterate before it reaches the "edge" -- that is, either the end element in the cycle[] array, or the beginning element. For instance, if cycle[] contains 4 elements, the end edge is at index 3, the beginning edge is at index 0.
														// if our target border line is at index 2, and we are going "forward", the firstIterationSetEdge would be 2, because the first set will iterate a total of 2 times before the set ends. The next set would begin at 0, and iterate 2 times.
														//
														// For FORWARD directions, the number of iterations for the first set is calculated as ((final index, in this case 3) - (beginning index, in this case 2)) + 1. 
		                                                //                         For the second set, the number of iterations is equal to (beginning index, in this case, 2).
														//                         The end result is that the first set begins at 2, iterates twice in a loop (so border lines at indices 2 and 3 are run in the set),
		                                                //                         and the second set begins at 0, and iterates twice (so indices 0 and 1).
		short secondIterationSetNumberOfIterations = 0;	// how many times the second set must iterate (see above)
		void calculateNumberOfIterations();
		int getTotalIterationCount();
};

#endif
