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

		//std::set<int> foundSet = intersectRecorderRef->getCleaveSequenceIDList();
		//foundSet = intersectRecorderRef->getCleaveSequenceIDList();
		foundSet = cleaveSequenceCandidateListRef->getCandidateSet();		// load the candidate set
		foundSet.erase(finderStartingCleaveSequenceID);						// remove the finder's starting cleave sequence from the found set.

		// testing only, print the lines.
		auto foundSetBegin = foundSet.begin();
		auto foundSetEnd = foundSet.end();
		for (; foundSetBegin != foundSetEnd; foundSetBegin++)
		{
			auto foundCleaveMap = (*cleaveMapRef).find(*foundSetBegin);
			foundCleaveMap->second.printCategorizedLines();
		}

		intersectRecorderRef->printRecords();
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
	// remember, the first value (int) of the std::map<int, BorderLineIntersectRecord>, -- aka "record" -- represents the CleaveSequenceID.

}