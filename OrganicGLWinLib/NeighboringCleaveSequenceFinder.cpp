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

	// first, find which CleaveSequences are in the CyclingDirection that we are going. To do this, get all the CleaveSequences specified in the
	// foundSet, and then do the following:
	//	1. For each one in the foundSet, get the DistanceToPoint values. 
	//	2. Put the DistanceToPoint values into a map of std::map<int, DistanceToPoint>
	//  3. translate a COPY of the sequenceFinderStartPoint to 0, 0, 0.
	//  4. Translate all of the closest points from each CleaveSequence AND either point A (CyclingDirection::REVERSE) or point B (CyclingDirection::FORWARD) of the borderline by the translation value from step 3.
	//  5. Get the amount of radians (via quaternion) to rotate the the selected border line point (point A, or point B) to positive y.
	//  6. Rotate all of the closest points AND the selected border line point by the radians in step 5.
	//  7. Only the closest points that are >= 0.0y after the translated are ones that should be considered, as the other ones all go the opposite direction.
	//     In other words, if we are going CyclingDirection::FORWARD, point B should be equal to Y = 1.0f after the radians are applied -- and any 
	//	   acceptable neighbor cleave sequences will have their closest points will be 0 < y <= 1.0f.


	std::map<int, DistanceToPoint> distanceToPointMap;
	auto foundSetBegin = foundSet.begin();
	auto foundSetEnd = foundSet.end();
	for (; foundSetBegin != foundSetEnd; foundSetBegin++)
	{
		DistanceToPoint shortestPointStats = (*cleaveMapRef)[*foundSetBegin].fetchClosestPoint(sequenceFinderStartPoint);
		distanceToPointMap[*foundSetBegin] = shortestPointStats;
		std::cout << ">> Inserted DistancePoint at index " << *foundSetBegin << ": distance-> " << shortestPointStats.distance << " | point-> " << shortestPointStats.point.x << ", " << shortestPointStats.point.y << ", " << shortestPointStats.point.z << std::endl;
	}
}