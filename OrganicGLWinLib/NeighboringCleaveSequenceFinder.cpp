#include "stdafx.h"
#include "NeighboringCleaveSequenceFinder.h"

void NeighboringCleaveSequenceFinder::buildNeighboringCleaveSequenceMap()
{
	// get a ref to the intersect recorder in our starting border line.
	BorderLineIntersectRecorder* intersectRecorderRef = &borderLineRef->intersectRecorder;

	// if it's the only intercept on this border line, that's ok.
	if (intersectRecorderRef->records.size() == 1)
	{
	}
	else if (intersectRecorderRef->records.size() > 1)
	{
		std::cout << "+++ Note: neighbors exist! " << std::endl;
		findAndSortNeighboringCleaveSequences();
		doNeighborsExist = true;
	}
}

bool NeighboringCleaveSequenceFinder::wereNeighborsFound()
{
	return doNeighborsExist;
}

void NeighboringCleaveSequenceFinder::findAndSortNeighboringCleaveSequences()
{

}