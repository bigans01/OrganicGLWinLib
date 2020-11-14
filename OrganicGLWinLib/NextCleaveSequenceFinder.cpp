#include "stdafx.h"
#include "NextCleaveSequenceFinder.h"

void NextCleaveSequenceFinder::buildNeighboringCleaveSequenceMap()
{
	//std::cout << "!!! *******BEGIN******** Sequence finder start point is: " << sequenceFinderStartPoint.x << ", " << sequenceFinderStartPoint.y << ", " << sequenceFinderStartPoint.z << std::endl;


	// if it's the only intercept on this border line, that's ok.
	BorderLineIntersectRecorder* intersectRecorderRef = &borderLineRef->intersectRecorder;
	if (intersectRecorderRef->records.size() == 1)
	{
		std::cout << ":::: entered branch for size == 1" << std::endl;

		foundSet = cleaveSequenceCandidateListRef->getCandidateSet();	
		if (foundSet.empty())
		{
			//std::cout << ">> The single CleaveSequence in this line has already been consumed, returning false." << std::endl;
			wasNextSequenceFound = false;
		}
		else if (!foundSet.empty())
		{
			auto foundSetBegin = foundSet.begin();
			//std::cout << "Set value: [" << *foundSetBegin << "]" << std::endl;
			DistanceToPoint shortestPointStats = (*cleaveMapRef)[*foundSetBegin].fetchClosestPoint(sequenceFinderStartPoint);	// there's only one sequence, so grab its DistanceToPoint value

			// because the distance between the sequenceFiderStartPoint and the shortest point isn't 0, we must insert a welded line.
			if (shortestPointStats.distance != 0.0f)	
			{
				FoundCleaveSequence selectedSequence(*foundSetBegin, shortestPointStats);
				WeldedLine newLine(sequenceFinderStartPoint, selectedSequence.cleaveSequenceTracingBeginPoint, borderLineRef->planarVector);
				weldedLinePoolRef->insertLineIntoPool(newLine);

				selectedCleaveSequenceMeta = selectedSequence;
				wasNextSequenceFound = true;		// this indicates that we actually found a legitimate neighbor to trace to.
			}

			// if the distance is 0, the shortestPointStats.point and sequenceFinderStartPoint are the same, so there is no line to produce.
			// we also need to return false so that the calling LineWelder will produce the rest of the border line as the next WeldedLine.
			// If the very first categorized line we start a LineWelder on is on a border line that contains just one record in its intersectRecorder, this would always be false.
			else
			{
				//std::cout << ":::: !! shortestPointStats.point and sequenceFinderStartPoint match, returning false. " << std::endl;
				wasNextSequenceFound = false;
			}
		}
	}
	else if (intersectRecorderRef->records.size() > 1)
	{
		//std::cout << "+++ Note: neighbors exist! " << std::endl;
		//std::cout << "+++ Value of finderStartingCleaveSequenceID: " << finderStartingCleaveSequenceID << std::endl;

		std::cout << ":::: entered branch for size > 1" << std::endl;
		//std::set<int> foundSet = intersectRecorderRef->getCleaveSequenceIDList();
		//foundSet = intersectRecorderRef->getCleaveSequenceIDList();
		foundSet = cleaveSequenceCandidateListRef->getCandidateSet();		// load the candidate set
		if (finderRunMode == LineWelderRunMode::CONTINUE)	// only do this if we're in "continue" mode
		{
			std::cout << "******** Performing continue mode check; will remove the ID of " << finderStartingCleaveSequenceID << " from the foundSet. " << std::endl;

			foundSet.erase(finderStartingCleaveSequenceID);						// remove the finder's starting cleave sequence from the found set.
		}
	
		// the set contains some things.
		if (!foundSet.empty())
		{

			//std::cout << "!!! Note, case 2 for when records size > 1 entered. " << std::endl;
			// testing only, print the lines.
			auto foundSetBegin = foundSet.begin();
			auto foundSetEnd = foundSet.end();
			for (; foundSetBegin != foundSetEnd; foundSetBegin++)
			{
				//std::cout << "Set value: [" << *foundSetBegin << "]" << std::endl;

				auto foundCleaveMap = (*cleaveMapRef).find(*foundSetBegin);
				//foundCleaveMap->second.printCategorizedLines();
			}

			//intersectRecorderRef->printRecords();
			findAndSortNeighboringCleaveSequences();
		}

		// if the set is empty, don't bother doing anything.
		else if (foundSet.empty())
		{
			//std::cout << "###### ++++ Found set was EMPTY. " << std::endl;
		}
	}
}

FoundCleaveSequence NextCleaveSequenceFinder::getSelectedCleaveSequenceMeta()
{
	return selectedCleaveSequenceMeta;
}

bool NextCleaveSequenceFinder::wasSequenceFound()
{
	return wasNextSequenceFound;
}

void NextCleaveSequenceFinder::findAndSortNeighboringCleaveSequences()
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
	//  8. After clipping of invalid candidates is done, get the CleaveSequence that has the shortest distance to the sequenceFinderStartPoint.


	std::map<int, DistanceToPoint> distanceToPointMap;
	auto foundSetBegin = foundSet.begin();
	auto foundSetEnd = foundSet.end();
	for (; foundSetBegin != foundSetEnd; foundSetBegin++)
	{
		DistanceToPoint shortestPointStats = (*cleaveMapRef)[*foundSetBegin].fetchClosestPoint(sequenceFinderStartPoint);		// Step 1
		distanceToPointMap[*foundSetBegin] = shortestPointStats;																// Step 2
		//std::cout << ">> Inserted DistancePoint at index " << *foundSetBegin << ": distance-> " << shortestPointStats.distance << " | point-> " << shortestPointStats.point.x << ", " << shortestPointStats.point.y << ", " << shortestPointStats.point.z << std::endl;
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

	glm::vec3 rotationTargetPoint = quatPoints.getLastPoint();
	//std::cout << ">>> Point to rotate to (pre translation) will be: " << rotationTargetPoint.x << ", " << rotationTargetPoint.y << ", " << rotationTargetPoint.z << std::endl;


	// check for translation on the sequenceFinderStartPoint
	PointTranslationCheck pointTranslationChecker;
	pointTranslationChecker.performCheck(sequenceFinderStartPoint);		// Step 3
	if (pointTranslationChecker.requiresTranslation == 1)
	{
		quatPoints.applyTranslation(pointTranslationChecker.getTranslationValue());		// Step 4
	}

	// find the radians needed to rotate the last point in quatPoints to positive Y -- if needed -- and then rotate
	QuatRotationManager rotationManager;
	rotationManager.initializeAndRunforAligningNeighboringCleaveSequencesToPosY(&quatPoints);			// Step 5 and Step 6
	rotationTargetPoint = quatPoints.getLastPoint();

	//std::cout << ">>> Point to rotate to (post translation, and rotation) will be: " << rotationTargetPoint.x << ", " << rotationTargetPoint.y << ", " << rotationTargetPoint.z << std::endl;
	
	// let's print whats in the distanceToPointMap, to make sure it's OK
	/*
	distanceToPointMapBegin = distanceToPointMap.begin();
	distanceToPointMapEnd = distanceToPointMap.end();
	for (; distanceToPointMapBegin != distanceToPointMapEnd; distanceToPointMapBegin++)
	{
		std::cout << "Quat-rotated point:           [" << distanceToPointMapBegin->first << "] -> " << distanceToPointMapBegin->second.point.x << ", " << distanceToPointMapBegin->second.point.y << ", " << distanceToPointMapBegin->second.point.z << std::endl;
		auto originalMapIter = originalCopy.find(distanceToPointMapBegin->first);
		std::cout << "Corresponding original point: [" << distanceToPointMapBegin->first << "] -> " << originalMapIter->second.point.x << ", " << originalMapIter->second.point.y << ", " << originalMapIter->second.point.z << std::endl;
	}
	*/

	// clip any point that isn't >= y = 0  (Step 7)
	std::vector<int> candidateCleaveSequencesToRemove;
	distanceToPointMapBegin = distanceToPointMap.begin();
	distanceToPointMapEnd = distanceToPointMap.end();
	for (; distanceToPointMapBegin != distanceToPointMapEnd; distanceToPointMapBegin++)
	{
		if ((distanceToPointMapBegin->second.point.y) < 0)	// if it's less than 0, it doesn't lie between the sequenceFinderStartPoint and the chosen end point of the border line,
															// so it must be clipped (erased)
		{
			candidateCleaveSequencesToRemove.push_back(distanceToPointMapBegin->first);
		}
	}
	auto removeBegin = candidateCleaveSequencesToRemove.begin();
	auto removeEnd = candidateCleaveSequencesToRemove.end();
	for (; removeBegin != removeEnd; removeBegin++)
	{
		distanceToPointMap.erase(*removeBegin);
	}

	// get the ID of the closest cleave sequence, and return the corresponding ID that was found in the originalCopy. (Step 8)
	float closestDistance = 1000.0f;	// start this out really high
	int currentShortestDistanceID = 0;
	distanceToPointMapBegin = distanceToPointMap.begin();
	distanceToPointMapEnd = distanceToPointMap.end();
	for (; distanceToPointMapBegin != distanceToPointMapEnd; distanceToPointMapBegin++)
	{
		if (distanceToPointMapBegin->second.distance < closestDistance)
		{
			closestDistance = distanceToPointMapBegin->second.distance;
			currentShortestDistanceID = distanceToPointMapBegin->first;
			wasNextSequenceFound = true;		// this indicates that we actually found a legitimate neighbor to trace to.
		}
	}

	// we found the shortest ID...
	FoundCleaveSequence selectedSequence(currentShortestDistanceID, originalCopy[currentShortestDistanceID]);
	selectedCleaveSequenceMeta = selectedSequence;
}