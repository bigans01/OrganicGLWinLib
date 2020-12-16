#include "stdafx.h"
#include "CleaveSequenceFactory.h"

void CleaveSequenceFactory::addCategorizedLine(CategorizedLine in_categorizedLine)
{
	if (in_categorizedLine.type == IntersectionType::A_SLICE)
	{
		insertAslicedLine(in_categorizedLine);
	}
	else if (in_categorizedLine.type == IntersectionType::PARTIAL_BOUND)
	{
		std::cout << "!!! Adding PARTIAL_BOUND line. " << std::endl;
		insertPartialBoundLine(in_categorizedLine);
	}
	else if (in_categorizedLine.type == IntersectionType::NON_BOUND)
	{
		std::cout << "!!! Adding NON_BOUND line. " << std::endl;
		insertNonboundLine(in_categorizedLine);
	}
	else if (in_categorizedLine.type == IntersectionType::INTERCEPTS_POINT_PRECISE)
	{
		std::cout << "!!!! Adding INTERCEPTS_POINT_PRECISE line" << std::endl;
		/*
		std::cout << "!!!! Adding INTERCEPTS_POINT_PRECISE line" << std::endl;
		std::cout << ":::: BEGIN ******************** Cycling direction and Border determination; border will be on point A********************************" << std::endl;
		std::cout << "(Pre-Alter) || ::: >> current categorized in_categorizedLine, point A: " << in_categorizedLine.line.pointA.x << ", " << in_categorizedLine.line.pointA.y << ", " << in_categorizedLine.line.pointA.z << std::endl;
		std::cout << "(Pre-Alter) || ::: >> current categorized in_categorizedLine.line, point A border: " << in_categorizedLine.line.pointABorder << std::endl;
		std::cout << "(Pre-Alter) || ::: >> current categorized in_categorizedLine.line, is point A on border: " << in_categorizedLine.line.isPointAOnBorder << std::endl;

		std::cout << "(Pre-Alter) || ::: >> current categorized in_categorizedLine.line, point B: " << in_categorizedLine.line.pointB.x << ", " << in_categorizedLine.line.pointB.y << ", " << in_categorizedLine.line.pointB.z << std::endl;
		std::cout << "(Pre-Alter) || ::: >> current categorized in_categorizedLine.line, point B border: " << in_categorizedLine.line.pointBBorder << std::endl;
		std::cout << "(Pre-Alter) || ::: >> current categorized in_categorizedLine.line, is point B on border: " << in_categorizedLine.line.isPointBOnBorder << std::endl;
		*/
		if (in_categorizedLine.line.pointA != in_categorizedLine.line.pointB)
		{
			insertInterceptsPointPrecise(in_categorizedLine);
		}
		else
		{
			//std::cout << "!!!! NOTICE: precise not inserted, due to the points being the same. " << std::endl;
		}

		//int someadd = 3;
		//std::cin >> someadd;
	}
	else
	{
		//std::cout << "!!!! WARNING, other line type discovered..." << std::endl;
	}
}

void CleaveSequenceFactory::copyCategorizedLinesFromLinePool(CategorizedLinePool* in_categorizedLinePoolRef)
{
	auto inPoolLinesBegin = in_categorizedLinePoolRef->linePool.begin();
	auto inPoolLinesEnd = in_categorizedLinePoolRef->linePool.end();
	for (; inPoolLinesBegin != inPoolLinesEnd; inPoolLinesBegin++)
	{
		addCategorizedLine(*inPoolLinesBegin);
	}
}

void CleaveSequenceFactory::insertNonboundLine(CategorizedLine in_line)
{
	nonboundMap[nonboundCount++] = in_line;
}

void CleaveSequenceFactory::insertPartialBoundLine(CategorizedLine in_line)
{
	partialboundMap[partialboundCount++] = in_line;
}

void CleaveSequenceFactory::insertAslicedLine(CategorizedLine in_line)
{
	aslicedMap[aslicedCount++] = in_line;
}

void CleaveSequenceFactory::insertInterceptsPointPrecise(CategorizedLine in_line)
{
	interceptsPointPreciseMap[interceptsPointPreciseCount++] = in_line;
}

void CleaveSequenceFactory::clipTwinCategorizedLinesofInterceptPointPrecise()
{
	std::map<int, int> twinCounterMap;
	auto interceptsPreciseBegin = interceptsPointPreciseMap.begin();
	auto interceptsPreciseEnd = interceptsPointPreciseMap.end();
	for (; interceptsPreciseBegin != interceptsPreciseEnd; interceptsPreciseBegin++)
	{
		twinCounterMap[interceptsPreciseBegin->second.parentPoly]++;
	}

	std::set<int> removalSet;
	auto twinCounterMapBegin = twinCounterMap.begin();
	auto twinCounterMapEnd = twinCounterMap.end();
	for (; twinCounterMapBegin != twinCounterMapEnd; twinCounterMapBegin++)
	{
		if (twinCounterMapBegin->second == 2)	// there's a twin found.
		{
			removalSet.insert(twinCounterMapBegin->first);		// insert the ID of the parent poly into the removal set, as we use this in the next step.
		}
	}

	std::vector<int> mappedValuesToRemove;
	auto removalRunBegin = interceptsPointPreciseMap.begin();
	auto removalRunEnd = interceptsPointPreciseMap.end();
	for (; removalRunBegin != removalRunEnd; removalRunBegin++)
	{
		auto wasFoundInSet = removalSet.find(removalRunBegin->second.parentPoly);
		if (wasFoundInSet != removalSet.end())
		{
			std::cout << "!!! Found a twin to remove..." << std::endl;
			mappedValuesToRemove.push_back(removalRunBegin->first);
		}
	}

	auto erasingBegin = mappedValuesToRemove.begin();
	auto erasingEnd = mappedValuesToRemove.end();
	for (; erasingBegin != erasingEnd; erasingBegin++)
	{
		interceptsPointPreciseMap.erase(*erasingBegin);
		interceptsPointPreciseCount--;
	}

	//std::cout << "!!! clipping complete, size of interceptsPointPreciseMap is: " << interceptsPointPreciseMap.size() << std::endl;

	//int someValAwYeah = 7;
	//std::cin >> someValAwYeah;
}



CategorizedLine CleaveSequenceFactory::fetchAndRemoveNonbound(int in_fetchIndex)
{
	CategorizedLine returnLine = nonboundMap[in_fetchIndex];
	nonboundMap.erase(in_fetchIndex);
	nonboundCount--;		// decrement the number of nonbound lines
	return returnLine;
}

CategorizedLine CleaveSequenceFactory::fetchAndRemovePartialBound(int in_fetchIndex)
{
	CategorizedLine returnLine = partialboundMap[in_fetchIndex];

	//std::cout << "************** Fetching and removing partial bound line, with points: " << std::endl;
	//std::cout << "### point A: " << returnLine.line.pointA.x << ", " << returnLine.line.pointA.y << ", " << returnLine.line.pointA.z << std::endl;
	//std::cout << "### point B: " << returnLine.line.pointB.x << ", " << returnLine.line.pointB.y << ", " << returnLine.line.pointB.z << std::endl;

	partialboundMap.erase(in_fetchIndex);
	partialboundCount--; // decrement the number of partial lines
	return returnLine;
}

CategorizedLine CleaveSequenceFactory::fetchAndRemoveASlice(int in_fetchIndex)
{
	CategorizedLine returnLine = aslicedMap[in_fetchIndex];
	aslicedMap.erase(in_fetchIndex);
	aslicedCount--;
	return returnLine;
}

CategorizedLine CleaveSequenceFactory::fetchAndRemoveInterceptPointPrecise(int in_fetchIndex)
{
	CategorizedLine returnLine = interceptsPointPreciseMap[in_fetchIndex];
	interceptsPointPreciseMap.erase(in_fetchIndex);
	interceptsPointPreciseCount--;
	return returnLine;
}

void CleaveSequenceFactory::constructAndExportCleaveSequences(std::map<int, CleaveSequence>* in_cleaveMapRef, std::map<int, SPolyBorderLines> in_borderLineArrayRef, MassManipulationMode in_massManipulationMode)
{
	// first, check if we need to invert the normals of each CategorizedLine in each CleaveSequence, in the event that the massManipulationMode of the SPoly is 
	// set to MassManipulationMode::DESTRUCTION

	//std::cout << "================================================>>>>>> calling constructAndExportCleaveSequences() " << std::endl;
	//std::cout << "number of nonbounds: " << nonboundCount << std::endl;
	//std::cout << "number of partials: " << partialboundCount << std::endl;
	//std::cout << "number of precises: " << interceptsPointPreciseCount << std::endl;

	if (in_massManipulationMode == MassManipulationMode::DESTRUCTION)
	{
		//std::cout << "!!!!!!! Destruction poly detected! " << std::endl;
		invertAllEmptyNormals();
	}
	
	// find the cycling directions for PARTIAL_BOUND and INTERSECTS_POINT_PRECISE. (will need to eventually include A_SLICE...)
	determineCyclingDirectionsForCategorizedLines(in_borderLineArrayRef);

	//std::cout << "(2) number of nonbounds: " << nonboundCount << std::endl;
	//std::cout << "(2) number of partials: " << partialboundCount << std::endl;
	//std::cout << "(2) number of precises: " << interceptsPointPreciseCount << std::endl;

	// Typical case 1: only do this if there are partial bound lines or a-sliced lines, and exactly 0 intereceptPointPreciseCount; this is the typical situation.
	if
	(
		((partialboundCount > 0) || (aslicedCount > 0))
		&&
		(interceptsPointPreciseCount == 0)	
	)
	{
		std::cout << ">>> Handling typical scenario" << std::endl;

		handleScenarioTypical(in_cleaveMapRef);
	}

	// Special case 1: there is 1 line with a value of INTERCEPTS_POINT_PRECISE.
	else if
	(
		(interceptsPointPreciseCount == 1)	// for a situation in which there is exactly one INTERCEPTS_POINT_PRECISE (this condition will change at a later date.)
	)
	{
		//std::cout << ":::: test: " << in_borderLineArrayRef[0].

		std::cout << ">>> Handling precise scenario" << std::endl;

		handleScenarioSingleInterceptsPointPreciseFound(in_cleaveMapRef, in_borderLineArrayRef);
	}
	else if
	(
		(interceptsPointPreciseCount > 1)
	)
	{
		std::cout << ">>> Handling multiple precise scenario" << std::endl;
		int stopVal = 3;
		std::cin >> stopVal;
	}
	else
	{
		std::cout << "!!!!!!!!!!!! Warning, invalid scenario detected, or number of intercept points precise has been reduced to 0...; " << std::endl;
		//std::cout << "!! Number of interceptsPointPrecise: " << interceptsPointPreciseCount << std::endl;
		//int someVal = 3;
		//std::cin >> someVal;
	}

	//std::cout << "================================================>>>>>> End call of constructAndExportCleaveSequences() " << std::endl;
}

void CleaveSequenceFactory::determineCyclingDirectionsForCategorizedLines(std::map<int, SPolyBorderLines> in_borderLineArrayRef)
{
	// go through each partial bound.

	//std::cout << "!!! Calling determineCyclingDirectionsForCategorizedLines" << std::endl;

	auto partialsBegin = partialboundMap.begin();
	auto partialsEnd = partialboundMap.end();
	for (; partialsBegin != partialsEnd; partialsBegin++)
	{
		//std::cout << ">>>>>>> Determining partial cycling direction..." << std::endl;
		partialsBegin->second.determineCyclingDirection(in_borderLineArrayRef);
	}

	// go through each intersects_point_precise
	//std::cout << ":::::::::: Points precise map size: " << interceptsPointPreciseMap.size() << std::endl;
	//std::cout << "....enter number to continue. " << std::endl;

	// check if there's a condition where there are exactly two categorized lines having the IntersectionType, INTERCEPTS_POINT_PRECISE, and 
	// which belong to the same parent poly. If this condition is TRUE, these lines must be removed.
	//clipTwinCategorizedLinesofInterceptPointPrecise();			// potentially obsolete function call

	//int someVal8 = 7;
	//std::cin >> someVal8;

	auto pointsPreciseBegin = interceptsPointPreciseMap.begin();
	auto pointsPreciseEnd = interceptsPointPreciseMap.end();
	for (; pointsPreciseBegin != pointsPreciseEnd; pointsPreciseBegin++)
	{
		//std::cout << ">>>>>>> Determining points precise cycling direction..." << std::endl;
		pointsPreciseBegin->second.determineCyclingDirection(in_borderLineArrayRef);
	}


	auto aslicedBegin = aslicedMap.begin();
	auto aslicedEnd = aslicedMap.end();
	for (; aslicedBegin != aslicedEnd; aslicedBegin++)
	{
		aslicedBegin->second.determineCyclingDirection(in_borderLineArrayRef);
	}

	//std::cout << "!!! Size of partialBoundMap: " << partialboundMap.size() << std::endl;
	//std::cout << "!!! Size of interceptsPointPreciseMap: " << interceptsPointPreciseMap.size() << std::endl;

	//std::cout << ":::::::: FINISHED determining cycling directions for PARTIAL_BOUND and INTERSECTS_POINT_PRECISE..." << std::endl;
	//int someVal = 3;
	//std::cin >> someVal;

}

void CleaveSequenceFactory::insertFirstPartialBoundLineForSequence(CleaveSequence* in_cleaveSequenceRef, int in_lineIndex)
{
	in_cleaveSequenceRef->insertFirstLine(fetchAndRemovePartialBound(in_lineIndex));
}

void CleaveSequenceFactory::insertASliceLineForSequence(CleaveSequence* in_cleaveSequenceRef, int in_lineIndex)
{
	in_cleaveSequenceRef->insertFirstLine(fetchAndRemoveASlice(in_lineIndex));
}

void CleaveSequenceFactory::invertAllEmptyNormals()
{
	// invert nonbounds
	auto nonBoundMapBegin = nonboundMap.begin();
	auto nonBoundMapEnd = nonboundMap.end();
	for (; nonBoundMapBegin != nonBoundMapEnd; nonBoundMapBegin++)
	{
		std::cout << "!! Inverting nonbound..." << std::endl;
		nonBoundMapBegin->second.emptyNormal *= -1.0f;
	}

	// invert partials
	auto partialsBegin = partialboundMap.begin();
	auto partialsEnd = partialboundMap.end();
	for (; partialsBegin != partialsEnd; partialsBegin++)
	{
		std::cout << "!! Inverting partial..." << std::endl;
		partialsBegin->second.emptyNormal *= -1.0f;
	}

	// invert slices
	auto slicesBegin = aslicedMap.begin();
	auto slicesEnd = aslicedMap.end();
	for (; slicesBegin != slicesEnd; slicesBegin++)
	{
		std::cout << "!! Inverting slice..." << std::endl;
		slicesBegin->second.emptyNormal *= -1.0f;
	}

	// invert intercept_points_precise
	auto interceptsPreciseBegin = interceptsPointPreciseMap.begin();
	auto interceptsPreciseEnd = interceptsPointPreciseMap.end();
	for (; interceptsPreciseBegin != interceptsPreciseEnd; interceptsPreciseBegin++)
	{
		std::cout << "!! Inverting intercept_points_precise..." << std::endl;
		interceptsPreciseBegin->second.emptyNormal *= -1.0f;
	}
}

CategorizedLineSearchResult CleaveSequenceFactory::searchForLastPartialBoundLineForSequence(glm::vec3 in_pointToSearch)
{
	CategorizedLineSearchResult searchResult;
	if (partialboundCount > 0)
	{
		// search through all the partially bound lines.
		auto partialboundBegin = partialboundMap.begin();
		auto partialboundEnd = partialboundMap.end();
		bool wasFound = false;		// was a match found?
		int foundIndex = 0;			// if it was found, what was the index?
		IRPointType pointCheckResult = IRPointType::NEITHER;	// starts out as NEITHER
		for (partialboundBegin; partialboundBegin != partialboundEnd; partialboundBegin++)
		{
			pointCheckResult = partialboundBegin->second.checkIfPointIsInLine(in_pointToSearch);
			if (pointCheckResult != IRPointType::NEITHER)
			{
				wasFound = true;
				foundIndex = partialboundBegin->first;
				break;
			}

			std::cout << "One iteration passed through partial bounds..." << std::endl;
		}

		// if it was found, do this stuff (should be able to ignore invalid iterators this way)
		if (wasFound == true)
		{
			searchResult.wasFound = true;
			CategorizedLine foundLine = fetchAndRemovePartialBound(foundIndex);
			// remember, when dealing with the ending partial line, the matched point should be "point A" of the border line; if it isn't, we must swap the line.
			if (pointCheckResult == IRPointType::POINT_B)
			{
				foundLine.line.swapToA();	// it's backwards; so swap the lines.

			}
			searchResult.returnLine = foundLine;	// store the appropriate line
			searchResult.nextPointToFind = foundLine.line.pointB;	// set the next point to find.
		}
	}
	else
	{
		searchResult.wasFound = false;
	}
	return searchResult;
}

CategorizedLineSearchResult CleaveSequenceFactory::searchForInterceptPointPreciseCategorizedLine(glm::vec3 in_pointToSearch, std::map<int, SPolyBorderLines>)
{
	CategorizedLineSearchResult searchResult;
	if (interceptsPointPreciseCount > 0)
	{
		// search through all the interceptPointPreciseLines.
		auto interceptPointPreciseBegin = interceptsPointPreciseMap.begin();
		auto interceptPointPreciseEnd = interceptsPointPreciseMap.end();
		bool wasFound = false;		// was a match found?
		int foundIndex = 0;
		IRPointType pointCheckResult = IRPointType::NEITHER;	// starts out as NEITHER
		for (interceptPointPreciseBegin; interceptPointPreciseBegin != interceptPointPreciseEnd; interceptPointPreciseBegin++)
		{
			std::cout << "!! Iterating through INTERCEPT_POINT_PRECISE map..." << std::endl;

			std::cout << "!! Line points are: " << std::endl;
			std::cout << "Point A: " << interceptPointPreciseBegin->second.line.pointA.x << ", " << interceptPointPreciseBegin->second.line.pointA.y << ", " << interceptPointPreciseBegin->second.line.pointA.z << std::endl;
			std::cout << "Point B: " << interceptPointPreciseBegin->second.line.pointB.x << ", " << interceptPointPreciseBegin->second.line.pointB.y << ", " << interceptPointPreciseBegin->second.line.pointB.z << std::endl;

			
			pointCheckResult = interceptPointPreciseBegin->second.checkIfPointIsInLine(in_pointToSearch);
			if (pointCheckResult != IRPointType::NEITHER)
			{
				wasFound = true;
				foundIndex = interceptPointPreciseBegin->first;
				//std::cout << "::::: found matching point to search, for a categorized line of type INTERCEPT_POINT_PRECISE! " << std::endl;
				break;

			}		
		}

		//std::cout << "hey I got here (1) " << std::endl;

		// if it was found, do this stuff (should be able to ignore invalid iterators this way)
		if (wasFound == true)
		{
			//std::cout << "hey I got here (2) " << std::endl;

			searchResult.wasFound = true;
			//std::cout << "::::: set searchResult.wasFound as true..." << std::endl;
			CategorizedLine foundLine = fetchAndRemoveInterceptPointPrecise(foundIndex);
			//CategorizedLine foundLine;

			// remember, when dealing with the ending partial line, the matched point should be "point A" of the border line; if it isn't, we must swap the line.
			if (pointCheckResult == IRPointType::POINT_B)
			{

				//std::cout << "(Pre-Alter) || ::: >> current categorized line, point A: " << foundLine.line.pointA.x << ", " << foundLine.line.pointA.y << ", " << foundLine.line.pointA.z << std::endl;
				//std::cout << "(Pre-Alter) || ::: >> current categorized line, point A border: " << foundLine.line.pointABorder << std::endl;
				//std::cout << "(Pre-Alter) || ::: >> current categorized line, is point A on border: " << foundLine.line.isPointAOnBorder << std::endl;

				//std::cout << "(Pre-Alter) || ::: >> current categorized line, point B: " << foundLine.line.pointB.x << ", " << foundLine.line.pointB.y << ", " << foundLine.line.pointB.z << std::endl;
				//std::cout << "(Pre-Alter) || ::: >> current categorized line, point B border: " << foundLine.line.pointBBorder << std::endl;
				//std::cout << "(Pre-Alter) || ::: >> current categorized line, is point B on border: " << foundLine.line.isPointBOnBorder << std::endl;



				//std::cout << "!!! Had to swap to A! " << std::endl;

				foundLine.line.swapToA();	// it's backwards; so swap the lines.

				//std::cout << "(Post-Alter) || ::: >> current categorized line, point A: " << foundLine.line.pointA.x << ", " << foundLine.line.pointA.y << ", " << foundLine.line.pointA.z << std::endl;
				//std::cout << "(Post-Alter) || ::: >> current categorized line, point A border: " << foundLine.line.pointABorder << std::endl;
				//std::cout << "(Post-Alter) || ::: >> current categorized line, is point A on border: " << foundLine.line.isPointAOnBorder << std::endl;

				//std::cout << "(Post-Alter) || ::: >> current categorized line, point B: " << foundLine.line.pointB.x << ", " << foundLine.line.pointB.y << ", " << foundLine.line.pointB.z << std::endl;
				//std::cout << "(Post-Alter) || ::: >> current categorized line, point B border: " << foundLine.line.pointBBorder << std::endl;
				//std::cout << "(Post-Alter) || ::: >> current categorized line, is point B on border: " << foundLine.line.isPointBOnBorder << std::endl;
			}

			/*
			std::cout << " **test** line empty normal: " << foundLine.emptyNormal.x << ", " << foundLine.emptyNormal.y << ", " << foundLine.emptyNormal.z << std::endl;

			std::cout << " **test** point A border: " << foundLine.line.pointABorder << std::endl;
			std::cout << " **test** point B border: " << foundLine.line.pointBBorder << std::endl;

			// fetching border lines...
			SPolyBorderLines borderLineA = in_borderLineArrayRef[foundLine.line.pointABorder];
			SPolyBorderLines borderLineB = in_borderLineArrayRef[foundLine.line.pointBBorder];

			// border line A:
			std::cout << "Border Line A points: A: " << borderLineA.pointA.x << ", " << borderLineA.pointA.y << ", " << borderLineA.pointA.z << " | B: "
				                                     << borderLineA.pointB.x << ", " << borderLineA.pointB.y << ", " << borderLineA.pointB.z << std::endl;

			// border line A:
			std::cout << "Border Line B points: A: " << borderLineB.pointA.x << ", " << borderLineB.pointA.y << ", " << borderLineB.pointA.z << " | B: "
													 << borderLineB.pointB.x << ", " << borderLineB.pointB.y << ", " << borderLineB.pointB.z << std::endl;

			generateManipulationDirectionsForIntersectsPointPrecise(borderLineA, foundLine.line.pointABorder, borderLineB, foundLine.line.pointBBorder, foundLine.emptyNormal);
			*/

			//foundLine.line.isPointBOnBorder = 1;		// after the swapToA -- if needed -- point B should be the one on the border line.

			searchResult.returnLine = foundLine;	// store the appropriate line
			searchResult.nextPointToFind = foundLine.line.pointB;	// set the next point to find.

			//break;
		}
	}
	else
	{
		searchResult.wasFound = false;
	}

	//std::cout << "~~~~~~ end of call to searchForInterceptPointPreciseCategorizedLine. " << std::endl;
	//int someVal = 3;
	//std::cin >> someVal;

	return searchResult;

}

std::map<MassManipulationMode, int> CleaveSequenceFactory::generateManipulationDirectionsForIntersectsPointPrecise(SPolyBorderLines in_borderLineA, int in_borderLineAID, SPolyBorderLines in_borderLineB, int in_borderLineBID, glm::vec3 in_categorizedLineNormal)
{
	std::map<MassManipulationMode, int> returnMap;
	// copy the border lines.
	SPolyBorderLines borderLineACopy = in_borderLineA;
	SPolyBorderLines borderLineBCopy = in_borderLineB;
	glm::vec3 emptyNormalCopy = in_categorizedLineNormal;

	// is in_borderLineA's point B equal to in_borderLineB's point A?



	QuatRotationPoints rotationPoints;
	glm::vec3 pointToTranslateAgainst;
	if (borderLineACopy.pointB == borderLineBCopy.pointA)
	{
		std::cout << "Line A links with Line B, at Line A's point B. " << std::endl;

		rotationPoints.pointsRefVector.push_back(&borderLineACopy.pointA);
		rotationPoints.pointsRefVector.push_back(&borderLineACopy.pointB);
		rotationPoints.pointsRefVector.push_back(&borderLineBCopy.pointB);

		pointToTranslateAgainst = borderLineACopy.pointB;

		//rotationPoints.pointsRefVector.push
	}
	// otherwise, it's the other way around.
	else if (borderLineBCopy.pointB == borderLineACopy.pointA)
	{
		std::cout << "Line B links with Line A, at Line B's point B. " << std::endl;

		rotationPoints.pointsRefVector.push_back(&borderLineBCopy.pointA);
		rotationPoints.pointsRefVector.push_back(&borderLineBCopy.pointB);
		rotationPoints.pointsRefVector.push_back(&borderLineACopy.pointB);

		//borderLineBCopy.pointB

		pointToTranslateAgainst = borderLineBCopy.pointB;
	}

	// do a translation check.
	PointTranslationCheck translationChecker;
	translationChecker.performCheck(pointToTranslateAgainst);
	if (translationChecker.requiresTranslation == 1)
	{
		rotationPoints.applyTranslation(translationChecker.getTranslationValue());
	}

	// now, add the normal at the end.
	rotationPoints.pointsRefVector.push_back(&emptyNormalCopy);

	//std::cout << ":::: Printing points: " << std::endl;
	//rotationPoints.printPoints();

	QuatRotationManager rotationManager;
	rotationManager.initializeAndRunForFindingBorderLine(&rotationPoints);

	// determine which point it is that is positive y (check the first and third points.)
	glm::vec3 candidateOne = rotationPoints.getPointByIndex(0);
	glm::vec3 candidateTwo = rotationPoints.getPointByIndex(2);
	glm::vec3 selectedPoint;
	if (candidateOne.y > 0)
	{
		selectedPoint = candidateOne;
	}
	else if (candidateTwo.y > 0)
	{
		selectedPoint = candidateTwo;
	}

	// take the selected point, find out where it belongs; then set the appropriate forward/reverse IDs.
	int forwardID, reverseID;

	/*
	if
	(
		(borderLineACopy.pointA == selectedPoint)
		||
		(borderLineACopy.pointB == selectedPoint)
	)
	{
		std::cout << "Border Line A is the FORWARD line. " << std::endl;

		forwardID = in_borderLineAID;
		reverseID = in_borderLineBID;
	}

	else if
	(
		(borderLineBCopy.pointA == selectedPoint)
		||
		(borderLineBCopy.pointB == selectedPoint)
	)
	{
		std::cout << "Border Line B is the FORWARD line. " << std::endl;

		forwardID = in_borderLineBID;
		reverseID = in_borderLineAID;
	}
	*/

	if
	(
		(borderLineACopy.pointA == selectedPoint)
	)
	{
		std::cout << "Border Line A is the FORWARD line, going towards point A (CyclingDirection REVERSE)" << std::endl;
		
		forwardID = in_borderLineAID;
		reverseID = in_borderLineBID;
	}
	else if
	(
		(borderLineACopy.pointB == selectedPoint)
	)
	{
		std::cout << "Border Line A is the FORWARD line, going towards point B (CyclingDirection FORWARD)" << std::endl;

		forwardID = in_borderLineAID;
		reverseID = in_borderLineBID;
	}
	else if
	(
		(borderLineBCopy.pointA == selectedPoint)
	)
	{
		std::cout << "Border Line B is the FORWARD line, going towards point A (CyclingDirection REVERSE)" << std::endl;

		forwardID = in_borderLineBID;
		reverseID = in_borderLineAID;
	}
	else if
	(
		(borderLineBCopy.pointB == selectedPoint)
	)
	{
		std::cout << "Border Line B is the FORWARD line, going towards point B (CyclingDirection FORWARD)" << std::endl;

		forwardID = in_borderLineBID;
		reverseID = in_borderLineAID;
	}
	

	std::cout << "Forward ID: " << forwardID << std::endl;
	std::cout << "Reverse ID: " << reverseID << std::endl;

	return returnMap;
}

CategorizedLineSearchResult CleaveSequenceFactory::checkForNextNonboundLine(glm::vec3 in_pointToSearch)
{
	CategorizedLineSearchResult searchResult;
	if (nonboundCount > 0)		// search for categorized lines, but only if there are ones to search for.
	{
		// search through all the non bound lines.
		auto nonboundBegin = nonboundMap.begin();
		auto nonboundEnd = nonboundMap.end();
		bool wasFound = false;	// was a match found? 
		int foundIndex = 0;		// if it was found, what was its index?
		IRPointType pointCheckResult = IRPointType::NEITHER;	// starts out as NEITHER
		for (nonboundBegin; nonboundBegin != nonboundEnd; nonboundBegin++)
		{
			pointCheckResult = nonboundBegin->second.checkIfPointIsInLine(in_pointToSearch);
			if (pointCheckResult != IRPointType::NEITHER) // it was found (it's either A or B)
			{
				wasFound = true;					// set the found flag.
				foundIndex = nonboundBegin->first;	// store the index.
				break;								// exit for loop when found.
			}

			//std::cout << "One iteration passed through nonbounds..." << std::endl;
		}

		// if it was found, do this stuff (should be able to ignore invalid iterators this way)
		if (wasFound == true)
		{
			searchResult.wasFound = true;			// set the search result found value to be true
			CategorizedLine foundLine = fetchAndRemoveNonbound(foundIndex);		// remove from the nonbound.
			if (pointCheckResult == IRPointType::POINT_B)
			{
				foundLine.line.swapToA();	// it's backwards; so swap the lines.

			}
			searchResult.returnLine = foundLine;	// store the appropriate line
			searchResult.nextPointToFind = foundLine.line.pointB;	// set the next point to find.


			//std::cout << "!! Point (next line) was found! " << std::endl;
			//std::cout << "Point A: " << foundLine.line.pointA.x << ", " << foundLine.line.pointA.y << ", " << foundLine.line.pointA.z << " | Point B: " << foundLine.line.pointB.x << ", " << foundLine.line.pointB.y << ", " << foundLine.line.pointB.z << std::endl;
			//std::cout << "Number of remaining nonbounds: " << nonboundCount << std::endl;
		}

	}
	else
	{
		searchResult.wasFound = false;
	}

	return searchResult;
}

void CleaveSequenceFactory::printLinesInPool()
{
	// print partially bound lines
	if (partialboundCount > 0)
	{
		auto begin = partialboundMap.begin();
		auto end = partialboundMap.end();
		std::cout << ">>> --- Partial lines: " << std::endl;
		for (begin; begin != end; begin++)
		{
			std::cout << begin->first << ": point A: " << begin->second.line.pointA.x << ", " << begin->second.line.pointA.y << ", " << begin->second.line.pointA.z 
								     << " | point B: " << begin->second.line.pointB.x << ", " << begin->second.line.pointB.y << ", " << begin->second.line.pointB.z 
				                     << " | empty normal: " << begin->second.emptyNormal.x << ", " << begin->second.emptyNormal.y << ", " << begin->second.emptyNormal.z << ", " << std::endl;
		}
	}

	// print nonbound lines
	if (nonboundCount > 0)
	{
		auto begin = nonboundMap.begin();
		auto end = nonboundMap.end();
		std::cout << ">>> --- Non-bound lines: " << std::endl;
		for (begin; begin != end; begin++)
		{
			std::cout << begin->first << ": point A: " << begin->second.line.pointA.x << ", " << begin->second.line.pointA.y << ", " << begin->second.line.pointA.z 
									 << " | point B: " << begin->second.line.pointB.x << ", " << begin->second.line.pointB.y << ", " << begin->second.line.pointB.z 
									<< " | empty normal: " << begin->second.emptyNormal.x << ", " << begin->second.emptyNormal.y << ", " << begin->second.emptyNormal.z << ", " << std::endl;
		}
	}

	// print a_slice lines
	if (aslicedCount > 0)
	{
		auto begin = aslicedMap.begin();
		auto end = aslicedMap.end();
		std::cout << ">>> --- Sliced lines: " << std::endl;
		for (; begin != end; begin++)
		{
			std::cout << begin->first << ": point A: " << begin->second.line.pointA.x << ", " << begin->second.line.pointA.y << ", " << begin->second.line.pointA.z 
									<< " | point B: " << begin->second.line.pointB.x << ", " << begin->second.line.pointB.y << ", " << begin->second.line.pointB.z 
									<< " | empty normal: " << begin->second.emptyNormal.x << ", " << begin->second.emptyNormal.y << ", " << begin->second.emptyNormal.z << ", " << std::endl;
		}
	}

	// intercepts point precise count
	if (interceptsPointPreciseCount > 0)
	{
		auto begin = interceptsPointPreciseMap.begin();
		auto end = interceptsPointPreciseMap.end();
		for (; begin != end; begin++)
		{
			std::cout << ">>> --- Intercepts point precise lines: " << std::endl;
			std::cout << begin->first << ": point A: " << begin->second.line.pointA.x << ", " << begin->second.line.pointA.y << ", " << begin->second.line.pointA.z 
									<< " | point B: " << begin->second.line.pointB.x << ", " << begin->second.line.pointB.y << ", " << begin->second.line.pointB.z 
									<< " | empty normal: " << begin->second.emptyNormal.x << ", " << begin->second.emptyNormal.y << ", " << begin->second.emptyNormal.z << ", " << std::endl;
		}
	}
}

void CleaveSequenceFactory::printLineCounts()
{
	std::cout << "--> Printing line counts in CleaveSequenceFactory: " << std::endl;
	std::cout << "nonbounds: " << nonboundCount << std::endl;
	std::cout << "partialBounds: " << partialboundCount << std::endl;
	std::cout << "precise: " << interceptsPointPreciseCount << std::endl;
	std::cout << "aslice: " << aslicedCount << std::endl;
}

void CleaveSequenceFactory::clearLinePools()
{
	nonboundMap.clear();
	nonboundCount = 0;

	partialboundMap.clear();
	partialboundCount = 0;

	interceptsPointPreciseMap.clear();
	interceptsPointPreciseCount = 0;

	aslicedMap.clear();
	aslicedCount = 0;
}

void CleaveSequenceFactory::handleScenarioTypical(std::map<int, CleaveSequence>* in_cleaveMapRef)
{
		//std::cout << "## Partial count: " << partialboundCount << std::endl;
		//std::cout << "## Non-bound count: " << nonboundCount << std::endl;
		//std::cout << "## Sliced count: " << aslicedCount << std::endl;

		// sliced checks.
	auto slicedBegin = aslicedMap.begin();
	auto slicedEnd = aslicedMap.end();
	for (; slicedBegin != slicedEnd; slicedBegin++)
	{
		std::cout << "++++++++++ Printing SLICED CategorizedLines: " << std::endl;
		std::cout << "Point A: " << slicedBegin->second.line.pointA.x << ", " << slicedBegin->second.line.pointA.y << ", " << slicedBegin->second.line.pointA.z << std::endl;
		std::cout << "Point B: " << slicedBegin->second.line.pointB.x << ", " << slicedBegin->second.line.pointB.y << ", " << slicedBegin->second.line.pointB.z << std::endl;
	}

	while (partialboundCount > 0)	// do this until all partial_bound lines have been accounted for. 
	{
		cleaveSequenceMapRef = in_cleaveMapRef;									// set the map reference that we will export results to.

		auto partialBoundMapBegin = partialboundMap.begin();					// get the first line in the partial bound map
		CategorizedLine* partialBoundLineRef = &partialBoundMapBegin->second;	// get a ref to the line
		int firstLineID = partialBoundMapBegin->first;							// store the ID of the first line (for removal later)
		CleaveSequence newSequence;												// the new line sequence that will eventually be inserted back into the referenced SPoly
		insertFirstPartialBoundLineForSequence(&newSequence, firstLineID);		// insert the first partial bound line we find
		glm::vec3 firstPointToSearch = newSequence.fetchPointToSearch();		// get the searchable point from the first partial bound line we found in the previous step
		//std::cout << "!!! Initial point to search is: " << firstPointToSearch.x << ", " << firstPointToSearch.y << ", " << firstPointToSearch.z << std::endl;


		// first, work with the partially bound lines. Get the first available partial bound line in the map, scan for linking nonbound lines, until no more linking lines are found.
		// Then, scan the partialBoundMap until the ending partially bound line is found (the B point of this partial line will equal the B point of a linked nonbound line). 
		// When this is done, take all the lines invovled (both nonbound and partial bound) and put them into a new CleaveSequence. Remmove their original copies from the appropriate maps --
		// partial bound lines will be removed from partialBoundMap (and the counter decremented) and the nonbound lines will be removed from the nonboundMap (also decrementing here)
		bool continueSearch = true;
		CategorizedLineSearchResult result = checkForNextNonboundLine(firstPointToSearch);	// search for the first point.
		if (result.wasFound == true)		// insert the first categorized line into the sequence, if it was found:
		{
			newSequence.insertNonboundLine(result.returnLine);	// insert the fetched line into the sequence
			bool continueFlag = true;							// check for the next line, at least once.
			while (continueFlag == true)	// loop until this is false.
			{
				glm::vec3 nextPointToSearch = newSequence.fetchPointToSearch();
				CategorizedLineSearchResult nextResult = checkForNextNonboundLine(nextPointToSearch);
				if (nextResult.wasFound == false)
				{
					continueFlag = false;	// end the loop.
				}
				else if (nextResult.wasFound == true)
				{
					newSequence.insertNonboundLine(nextResult.returnLine);
				}
			}
		}

		// once the search for partially bound lines is done, look for the ending partially bound line for the sequence.
		glm::vec3 lastPointToSearch = newSequence.fetchPointToSearch();
		//std::cout << "### Current last point to search is: " << lastPointToSearch.x << ", " << lastPointToSearch.y << ", " << lastPointToSearch.z << std::endl;
		CategorizedLineSearchResult finalResult = searchForLastPartialBoundLineForSequence(lastPointToSearch);
		if (finalResult.wasFound == true)
		{
			//std::cout << "Final partial bound line found! Inserting final line... !!" << std::endl;
			newSequence.insertLastLine(finalResult.returnLine);
			newSequence.sequenceStatus = CleaveSequenceStatus::COMPLETE; // mark it as complete
		}
		else
		{
			//std::cout << "!! Final partial bound line NOT FOUND! " << std::endl;
			newSequence.sequenceStatus = CleaveSequenceStatus::INCOMPLETE; // mark it as complete

			std::cout << "Warning, CleaveSequence is INCOMPLETE. " << std::endl;
			std::cout << "Lines are: " << std::endl;
			newSequence.printCategorizedLines();

			int someVal = 3;
			std::cin >> someVal;
		}
		//std::cout << "## Remaining number of partial bounds: " << partialboundCount << std::endl;


		// lastly, if the newSequence is marked as "complete" move it to the referenced sequence. Otherwise, discard it.
		if (newSequence.sequenceStatus == CleaveSequenceStatus::COMPLETE)
		{
			int cleaveMapRefSize = int((*in_cleaveMapRef).size());
			//std::cout << "!! Inserting new cleave sequence at index: " << cleaveMapRefSize << std::endl;
			(*in_cleaveMapRef)[cleaveMapRefSize] = newSequence;	// insert the sequence.
			//std::cout << "Map size is now: " << cleaveMapRefSize << std::endl;
		}
	}

	while (aslicedCount > 0)
	{
		std::cout << "STOP! It's hammer time. " << std::endl;

		cleaveSequenceMapRef = in_cleaveMapRef;									// set the map reference that we will export results to.
		auto aslicedMapBegin = aslicedMap.begin();					// get the first line in the a_slice map
		int firstLineID = aslicedMapBegin->first;							// store the ID of the first line (for removal later)
		CleaveSequence newSequence;
		insertASliceLineForSequence(&newSequence, firstLineID);
		int cleaveMapRefSize = int((*in_cleaveMapRef).size());
		(*in_cleaveMapRef)[cleaveMapRefSize] = newSequence;	// insert the sequence.

		newSequence.printCategorizedLines();

		int someVal = 3;
		std::cin >> someVal;


	}
}

void CleaveSequenceFactory::handleScenarioSingleInterceptsPointPreciseFound(std::map<int, CleaveSequence>* in_cleaveMapRef, std::map<int, SPolyBorderLines> in_borderLineArrayRef)
{
	
	//std::cout << "###::::: !!!! Running specical case for INTERCEPTS_POINT_PRECISE. " << std::endl;
	//std::cout << "## Partial count: " << partialboundCount << std::endl;
	//std::cout << "## Non-bound count: " << nonboundCount << std::endl;
	//std::cout << "## Sliced count: " << aslicedCount << std::endl;
	//std::cout << "## Intercepts precise count: " << interceptsPointPreciseCount << std::endl;
	//int someVal = 3;
	//std::cin >> someVal;

	// first, work with the partailly bound searching like we would normally do, except for the fact that 
	// when we are done, do not mark the CleaveSequenceStatus as being incomplete.

	glm::vec3 lastPointToSearch;
	while (partialboundCount > 0)	// do this until all partial_bound lines have been accounted for. 
	{
		cleaveSequenceMapRef = in_cleaveMapRef;									// set the map reference that we will export results to.

		auto partialBoundMapBegin = partialboundMap.begin();					// get the first line in the partial bound map
		CategorizedLine* partialBoundLineRef = &partialBoundMapBegin->second;	// get a ref to the line
		int firstLineID = partialBoundMapBegin->first;							// store the ID of the first line (for removal later)
		CleaveSequence newSequence;												// the new line sequence that will eventually be inserted back into the referenced SPoly
		insertFirstPartialBoundLineForSequence(&newSequence, firstLineID);		// insert the first partial bound line we find
		glm::vec3 firstPointToSearch = newSequence.fetchPointToSearch();		// get the searchable point from the first partial bound line we found in the previous step
		//std::cout << "!!! Initial point to search is: " << firstPointToSearch.x << ", " << firstPointToSearch.y << ", " << firstPointToSearch.z << std::endl;


		// first, work with the partially bound lines. Get the first available partial bound line in the map, scan for linking nonbound lines, until no more linking lines are found.
		// Then, scan the partialBoundMap until the ending partially bound line is found (the B point of this partial line will equal the B point of a linked nonbound line). 
		// When this is done, take all the lines invovled (both nonbound and partial bound) and put them into a new CleaveSequence. Remmove their original copies from the appropriate maps --
		// partial bound lines will be removed from partialBoundMap (and the counter decremented) and the nonbound lines will be removed from the nonboundMap (also decrementing here)
		bool continueSearch = true;
		CategorizedLineSearchResult result = checkForNextNonboundLine(firstPointToSearch);	// search for the first point.
		if (result.wasFound == true)		// insert the first categorized line into the sequence, if it was found:
		{
			newSequence.insertNonboundLine(result.returnLine);	// insert the fetched line into the sequence
			bool continueFlag = true;							// check for the next line, at least once.
			while (continueFlag == true)	// loop until this is false.
			{
				glm::vec3 nextPointToSearch = newSequence.fetchPointToSearch();
				CategorizedLineSearchResult nextResult = checkForNextNonboundLine(nextPointToSearch);
				if (nextResult.wasFound == false)
				{
					continueFlag = false;	// end the loop.
				}
				else if (nextResult.wasFound == true)
				{
					newSequence.insertNonboundLine(nextResult.returnLine);
				}
			}
		}

		// once the search for non bound lines is done, look for the ending partially bound line for the sequence.
		lastPointToSearch = newSequence.fetchPointToSearch();
		//std::cout << "### Current last point to search is: " << lastPointToSearch.x << ", " << lastPointToSearch.y << ", " << lastPointToSearch.z << std::endl;
		CategorizedLineSearchResult finalResult = searchForLastPartialBoundLineForSequence(lastPointToSearch);
		if (finalResult.wasFound == true)
		{
			//std::cout << "Final partial bound line found! Inserting final line... !!" << std::endl;
			newSequence.insertLastLine(finalResult.returnLine);
			newSequence.sequenceStatus = CleaveSequenceStatus::COMPLETE; // mark it as complete
		}
		else
		{
			//std::cout << "!! Final partial bound line NOT FOUND! " << std::endl;
			//newSequence.sequenceStatus = CleaveSequenceStatus::INCOMPLETE; // mark it as complete

			//std::cout << "Warning, a corresponding PARTIAL_BOUND line was not found for this line; checking for INTERCEPT_POINT_PRECISE: " << std::endl;

			//std::cout << "::: Lines are: " << std::endl;
			//newSequence.printCategorizedLines();

			//std::cout << ">>>> Current last point to search for against the INTERCEPTS_POINT_PRECISE IS: " << lastPointToSearch.x << ", " << lastPointToSearch.y << ", " << lastPointToSearch.z << std::endl;
			CategorizedLineSearchResult findInterceptPointPreciseResult = searchForInterceptPointPreciseCategorizedLine(lastPointToSearch, in_borderLineArrayRef);
			//std::cout << ">>>> Found intercept point precise result... " << std::endl;
			if (findInterceptPointPreciseResult.wasFound == true)
			{
				// before we insert the line, the pointB on border value for this new line must be equal to the pointA border value in the first part of the sequence.
				// this is because 
				auto firstLineInSequence = newSequence.cleavingLines.begin();
				//int borderIDForInterceptPointPrecise = firstLineInSequence->second.line.pointABorder;
				//std::cout << ">>> :::: Border value to set is: " << borderIDForInterceptPointPrecise << std::endl;
				//findInterceptPointPreciseResult.returnLine.line.pointBBorder = borderIDForInterceptPointPrecise;

				//std::cout << "Final partial bound line found! Inserting final line... !!" << std::endl;
				newSequence.insertLastLine(findInterceptPointPreciseResult.returnLine);
				newSequence.sequenceStatus = CleaveSequenceStatus::COMPLETE; // mark it as complete
			}
			//int someVal = 3;
			//std::cin >> someVal;
		}

		// lastly, if the newSequence is marked as "complete" move it to the referenced sequence. Otherwise, discard it.
		if (newSequence.sequenceStatus == CleaveSequenceStatus::COMPLETE)
		{
			int cleaveMapRefSize = int((*in_cleaveMapRef).size());
			//std::cout << "!! Inserting new cleave sequence at index: " << cleaveMapRefSize << std::endl;
			(*in_cleaveMapRef)[cleaveMapRefSize] = newSequence;	// insert the sequence.
			//std::cout << "Map size is now: " << cleaveMapRefSize << std::endl;
		}
	}
	
	//std::cout << "Remaining partial bound count: " << partialboundCount << std::endl;
	//std::cout << "Remaining nonbound count: " << nonboundCount << std::endl;
	//std::cout << "Remaining intercepts point count: " << interceptsPointPreciseCount << std::endl;
	
	//int someVal = 3;
	//std::cout << "Fix this shit, compiler. " << std::endl;
	//std::cin >> someVal;
}
