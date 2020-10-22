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
	//  3. translate a COPY of the sequenceFinderStartPoint to 0, 0, 0; this will not be needed if the point is at 0, 0, 0 already.
	//  4. Translate all of the closest points from each CleaveSequence, AND either point A (CyclingDirection::REVERSE) or point B (CyclingDirection::FORWARD) of the borderline by the translation value from step 3.
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
		DistanceToPoint shortestPointStats = (*cleaveMapRef)[*foundSetBegin].fetchClosestPoint(sequenceFinderStartPoint);		// Step 1
		distanceToPointMap[*foundSetBegin] = shortestPointStats;																// Step 2
		std::cout << ">> Inserted DistancePoint at index " << *foundSetBegin << ": distance-> " << shortestPointStats.distance << " | point-> " << shortestPointStats.point.x << ", " << shortestPointStats.point.y << ", " << shortestPointStats.point.z << std::endl;
	}

	std::map<int, DistanceToPoint> originalCopy = distanceToPointMap;	// create a copy of the map, so that when we find the nearest CleaveSequence by its ID, 
																		// we can get the pre-quaternion value (if we end up getting anything at all)

	

	// load the points from the DistanceToPoint values.
	QuatRotationPoints quatPoints;
	auto distanceToPointMapBegin = distanceToPointMap.begin();
	auto distanceToPointMapEnd = distanceToPointMap.end();
	for (; distanceToPointMapBegin != distanceToPointMapEnd; distanceToPointMapBegin++)
	{
		quatPoints.pointsRefVector.push_back(&distanceToPointMapBegin->second.point);
	}

	// next, determine which point of the border line to rotate to, based on the CyclingDirection
	glm::vec3 selectedPointOfBorderLine;					// get a COPY 
	if (cyclingDirection == CyclingDirection::FORWARD)
	{
		selectedPointOfBorderLine = borderLineRef->pointB;	// a FORWARD direction means we must use point B.
	}
	else if (cyclingDirection == CyclingDirection::REVERSE)
	{
		selectedPointOfBorderLine = borderLineRef->pointA;	// a REVERSE direction means we must use point A.
	}

	// push the value we just determined into the quat points. This should always be the last value in the vector.
	quatPoints.pointsRefVector.push_back(&selectedPointOfBorderLine);

	//glm::vec3 rotationTargetPoint = quatPoints.getLastPoint();
	//std::cout << ">>> Point to rotate to (pre translation) will be: " << rotationTargetPoint.x << ", " << rotationTargetPoint.y << ", " << rotationTargetPoint.z << std::endl;


	// check for translation on the sequenceFinderStartPoint
	PointTranslationCheck pointTranslationChecker;
	pointTranslationChecker.performCheck(sequenceFinderStartPoint);		// Step 3
	if (pointTranslationChecker.requiresTranslation == 1)
	{
		quatPoints.applyTranslation(pointTranslationChecker.getTranslationValue());		// Step 4
	}

	// find the radians needed to rotate the last point in quatPoints to positive Y, if needed.
	glm::vec3 rotationTargetPoint = quatPoints.getLastPoint();
	//std::cout << ">>> Point to rotate to (post translation) will be: " << rotationTargetPoint.x << ", " << rotationTargetPoint.y << ", " << rotationTargetPoint.z << std::endl;
	
	
}