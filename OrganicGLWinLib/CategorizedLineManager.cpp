#include "stdafx.h"
#include "CategorizedLineManager.h"

void CategorizedLineManager::setCategorizedLineManagerDebugLevel(PolyDebugLevel in_polyDebugLevel)
{
	categorizedLineManagerLogger.setDebugLevel(in_polyDebugLevel);
	categorizedLineManagerLoggerDebugLevel = in_polyDebugLevel;
}

int CategorizedLineManager::getCountOfIntersectionType(IntersectionType in_intersectionTypeToSearch)
{
	return containerMap[in_intersectionTypeToSearch].getLineCount();
}

bool CategorizedLineManager::checkForDuplicateCategorizedLine(CategorizedLine in_duplicateToCheck)
{
	bool duplicateFound = false;
	auto containerMapBegin = containerMap.begin();
	auto containerMapEnd = containerMap.end();
	for (; containerMapBegin != containerMapEnd; containerMapBegin++)
	{
		auto currentContainerLineBegin = containerMapBegin->second.lineMap.begin();
		auto currentContainerLineEnd = containerMapBegin->second.lineMap.end();
		for (; currentContainerLineBegin != currentContainerLineEnd; currentContainerLineBegin++)
		{
			if
			(
				(
					(in_duplicateToCheck.line.pointA == currentContainerLineBegin->second.line.pointA)
					&&
					(in_duplicateToCheck.line.pointB == currentContainerLineBegin->second.line.pointB)
				)

				||

				(
					(in_duplicateToCheck.line.pointA == currentContainerLineBegin->second.line.pointB)
					&&
					(in_duplicateToCheck.line.pointB == currentContainerLineBegin->second.line.pointA)
				)


			)
			{
				/*
				std::cout << "!! Duplicate line found; points are -> A: " << in_duplicateToCheck.line.pointA.x << ", "
					<< in_duplicateToCheck.line.pointA.y << ", "
					<< in_duplicateToCheck.line.pointA.z << "| B "
					<< in_duplicateToCheck.line.pointB.x << ", "
					<< in_duplicateToCheck.line.pointB.y << ", "
					<< in_duplicateToCheck.line.pointB.z << std::endl;
				*/
				duplicateFound = true;
			}
		}
	}
	return duplicateFound;
}

int CategorizedLineManager::insertLineAndReturnInsertedIndex(CategorizedLine in_categorizedLine)
{
	// use the type of the line to perform the insert.
	int insertedAtIndex = containerMap[in_categorizedLine.type].insertLineAndReturnInsertedIndex(in_categorizedLine);	// insert the line, use the index to insert into managerGrpMap.
	managerGrpMap.insertGroupRecord(in_categorizedLine.parentPoly, in_categorizedLine.type, insertedAtIndex);		// insert appropriate record into the managerGrpMap.
	return insertedAtIndex;	// return the inserted index value.
}

CategorizedLine CategorizedLineManager::fetchAndRemoveLineAtIndex(IntersectionType in_intersectionType, int in_targetIndex)
{
	return containerMap[in_intersectionType].fetchAndRemove(in_targetIndex);
}

CategorizedLine CategorizedLineManager::fetchAndRemoveWithGroupMapLocationPush(IntersectionType in_intersectionType,
	int in_fetchIndex,
	std::vector<CategorizedLineGroupLocation>* in_categorizedLineGroupLocationVectorRef)
{
	CategorizedLine returnLine = fetchAndRemoveLineAtIndex(in_intersectionType, in_fetchIndex);
	CategorizedLineGroupLocation location = managerGrpMap.fetchGroupRecordLocation(returnLine.parentPoly, in_intersectionType, in_fetchIndex);
	in_categorizedLineGroupLocationVectorRef->push_back(location);
	return returnLine;
}

bool CategorizedLineManager::doAnyLinesExist()
{
	bool linesExist = false;
	int totalLineCount = 0;
	auto containerMapBegin = containerMap.begin();
	auto containerMapEnd = containerMap.end();
	for (; containerMapBegin != containerMapEnd; containerMapBegin++)
	{
		totalLineCount += containerMapBegin->second.getLineCount();
	}
	if (totalLineCount > 0)
	{
		linesExist = true;
	}
	return linesExist;
}

void CategorizedLineManager::printLineCountsForEachType()
{
	std::cout << "(CategorizedLineManager): Printing line counts..." << std::endl;
	auto containerMapBegin = containerMap.begin();
	auto containerMapEnd = containerMap.end();
	for (; containerMapBegin != containerMapEnd; containerMapBegin++)
	{
		switch (containerMapBegin->first)
		{
			case IntersectionType::A_SLICE: { std::cout << "A_SLICE: " << containerMapBegin->second.getLineCount() << std::endl; break; }
			case IntersectionType::A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE: { std::cout << "A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE: " << containerMapBegin->second.getLineCount() << std::endl; break; }
			case IntersectionType::INTERCEPTS_POINT_PRECISE: { std::cout << "INTERCEPTS_POINT_PRECISE: " << containerMapBegin->second.getLineCount() << std::endl; break; }
			case IntersectionType::NON_BOUND: { std::cout << "NON_BOUND: " << containerMapBegin->second.getLineCount() << std::endl; break; }
			case IntersectionType::PARTIAL_BOUND: { std::cout << "PARTIAL_BOUND: " << containerMapBegin->second.getLineCount() << std::endl; break; }
		}
	}
}

void CategorizedLineManager::printAllLines()
{
	std::cout << "(CategorizedLineManager): Printing all line metadata..." << std::endl;
	auto containerMapBegin = containerMap.begin();
	auto containerMapEnd = containerMap.end();
	for (; containerMapBegin != containerMapEnd; containerMapBegin++)
	{
		if (containerMapBegin->second.lineMap.size() != 0)	// only print for a group if it has lines
		{
			switch (containerMapBegin->first)
			{
				case IntersectionType::A_SLICE: { std::cout << "A_SLICE: " << std::endl; break; }
				case IntersectionType::A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE: { std::cout << "A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE: " << std::endl; break; }
				case IntersectionType::INTERCEPTS_POINT_PRECISE: { std::cout << "INTERCEPTS_POINT_PRECISE: " << std::endl; break; }
				case IntersectionType::NON_BOUND: { std::cout << "NON_BOUND: " << std::endl; break; }
				case IntersectionType::PARTIAL_BOUND: { std::cout << "PARTIAL_BOUND: " << std::endl; break; }
			}
			auto begin = containerMapBegin->second.lineMap.begin();
			auto end = containerMapBegin->second.lineMap.end();
			for (begin; begin != end; begin++)
			{
				std::cout << begin->first << ": parent SPoly: " << begin->second.parentPoly << " | "
					<< ": point A: " << begin->second.line.pointA.x << ", " << begin->second.line.pointA.y << ", " << begin->second.line.pointA.z
					<< " | point B: " << begin->second.line.pointB.x << ", " << begin->second.line.pointB.y << ", " << begin->second.line.pointB.z
					<< " | empty normal: " << begin->second.emptyNormal.x << ", " << begin->second.emptyNormal.y << ", " << begin->second.emptyNormal.z << ", " << std::endl;
			}
		}
	}
}

std::map<int, CategorizedLine>::iterator CategorizedLineManager::getBeginIteratorForType(IntersectionType in_intersectionType)
{
	return containerMap[in_intersectionType].lineMap.begin();
}

std::map<int, CategorizedLine>::iterator CategorizedLineManager::getEndIteratorForType(IntersectionType in_intersectionType)
{
	return containerMap[in_intersectionType].lineMap.end();
}

void CategorizedLineManager::clearAllLines()
{
	auto containerMapBegin = containerMap.begin();
	auto containerMapEnd = containerMap.end();
	for (; containerMapBegin != containerMapEnd; containerMapBegin++)
	{
		containerMapBegin->second.lineMap.clear();
	}
}

void CategorizedLineManager::invertEmptyNormalsInContainers()
{
	auto containerMapBegin = containerMap.begin();
	auto containerMapEnd = containerMap.end();
	for (; containerMapBegin != containerMapEnd; containerMapBegin++)
	{
		containerMapBegin->second.invertNormals();
	}
}

void CategorizedLineManager::loadCategorizedLineEmptyNormals(QuatRotationPoints* in_quatRotationPointsRef)
{
	auto containerMapBegin = containerMap.begin();
	auto containerMapEnd = containerMap.end();
	for (; containerMapBegin != containerMapEnd; containerMapBegin++)
	{
		auto currentLineBegin = containerMapBegin->second.lineMap.begin();
		auto currentLineEnd = containerMapBegin->second.lineMap.end();
		for (; currentLineBegin != currentLineEnd; currentLineBegin++)
		{
			in_quatRotationPointsRef->insertPointRefs(&currentLineBegin->second.emptyNormal);
		}
	}
}

void CategorizedLineManager::loadCategorizedLinePoints(QuatRotationPoints* in_quatRotationPointsRef)
{
	auto containerMapBegin = containerMap.begin();
	auto containerMapEnd = containerMap.end();
	for (; containerMapBegin != containerMapEnd; containerMapBegin++)
	{
		auto currentLineBegin = containerMapBegin->second.lineMap.begin();
		auto currentLineEnd = containerMapBegin->second.lineMap.end();
		for (; currentLineBegin != currentLineEnd; currentLineBegin++)
		{
			in_quatRotationPointsRef->insertPointRefs(&currentLineBegin->second.line.pointA, &currentLineBegin->second.line.pointB);
		}
	}
}

bool CategorizedLineManager::determineCyclingDirections(std::map<int, SPolyBorderLines> in_borderLineArrayRef, PolyDebugLevel in_polyDebugLevel)
{
	auto containerMapBegin = containerMap.begin();
	auto containerMapEnd = containerMap.end();
	bool wasEntireRunValid = true;
	for (; containerMapBegin != containerMapEnd; containerMapBegin++)
	{
		auto currentLineBegin = containerMapBegin->second.lineMap.begin();
		auto currentLineEnd = containerMapBegin->second.lineMap.end();
		for (; currentLineBegin != currentLineEnd; currentLineBegin++)
		{
			bool currentRunValidity = currentLineBegin->second.determineCyclingDirection(in_borderLineArrayRef, in_polyDebugLevel);
			if (currentRunValidity == false)
			{
				wasEntireRunValid = false;
			}
		}
	}
	return wasEntireRunValid;
}

CategorizedLineSearchResult CategorizedLineManager::checkManagerForNextNonboundLine(glm::vec3 in_pointToSearch)
{
	categorizedLineManagerLogger.log("(CategorizedLineManager): Checking for next non-bound line, that has this point: ", in_pointToSearch.x, ",", in_pointToSearch.y, ", ", in_pointToSearch.z, "\n");
	CategorizedLineSearchResult searchResult;

	//std::cout << ">>>>>>>>>>>>>>>> Printing NON_BOUND lines...." << std::endl;
	//containerMap[IntersectionType::NON_BOUND].printLines();

	if (containerMap[IntersectionType::NON_BOUND].lineMap.size() > 0)	// search for NON_BOUND, categorized lines, but only if there are ones to search for.
	{
		// search through all the non bound lines.
		auto nonboundBegin = containerMap[IntersectionType::NON_BOUND].lineMap.begin();
		auto nonboundEnd = containerMap[IntersectionType::NON_BOUND].lineMap.end();
		bool wasFound = false;	// was a match found? 
		int foundIndex = 0;		// if it was found, what was its index?
		IRPointType pointCheckResult = IRPointType::NEITHER;	// starts out as NEITHER
		for (nonboundBegin; nonboundBegin != nonboundEnd; nonboundBegin++)
		{
			//std::cout << ">>>> printing NON-BOUND line.." << std::endl;
			//std::cout << nonboundBegin->second.line.pointA.x << ", " << nonboundBegin->second.line.pointA.y << ", " << nonboundBegin->second.line.pointA.z << std::endl;
			//std::cout << nonboundBegin->second.line.pointB.x << ", " << nonboundBegin->second.line.pointB.y << ", " << nonboundBegin->second.line.pointB.z << std::endl;

			categorizedLineManagerLogger.log("(CategorizedLineManager): Checking non bound line. ", "\n");
			pointCheckResult = nonboundBegin->second.checkIfPointIsInLine(in_pointToSearch);
			//
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
			searchResult.wasFound = true;																		// set the search result found value to be true
			CategorizedLine foundLine = fetchAndRemoveLineAtIndex(IntersectionType::NON_BOUND, foundIndex);		// fetch and remove from the NON_BOUND pool, at the index.
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

		// error/bad calculation handling; 9/6/2021 -- expand the point search "radius" by .2f;
		// if we find a line nearby, return a "modified" version of that line, and remove the original line
		// we based the altering off of from the corresponding pool.
		else if (wasFound == false)
		{
			std::cout << "!!! Notice -> wasFound flagged as false, running backup option..." << std::endl;
			std::cout << "pointToSearch is: " << in_pointToSearch.x << ", " << in_pointToSearch.y << ", " << in_pointToSearch.z << std::endl;
			searchResult.backupOptionFlag = true;

			auto nonBoundSecondPassBegin = containerMap[IntersectionType::NON_BOUND].lineMap.begin();
			auto nonBoundSecondPassEnd = containerMap[IntersectionType::NON_BOUND].lineMap.end();
			for (; nonBoundSecondPassBegin != nonBoundSecondPassEnd; nonBoundSecondPassBegin++)
			{
				pointCheckResult = nonBoundSecondPassBegin->second.checkIfPointIsNearbyPointInLine(in_pointToSearch, 0.02f);
				if (pointCheckResult != IRPointType::NEITHER) // it was found (it's either A or B)
				{
					//std::cout << "!!! NOTICE: found nearby point...continue? " << std::endl;
					//int nearbyFound = 3;
					//std::cin >> nearbyFound;

					std::cout << "(CategorizedLineManager::checkManagerForNextNonboundLine): NOTICE: found nearby point; continuing. " << std::endl;

					// because we found a nearby non-bound line, we must 
					// if we matched against point B of the non bound, then change point B of the line to be 
					searchResult.returnLine = nonBoundSecondPassBegin->second;	// insert the line...
					if (pointCheckResult == IRPointType::POINT_B)
					{
						searchResult.returnLine.line.pointB = in_pointToSearch;
						searchResult.nextPointToFind = searchResult.returnLine.line.pointA;	// next point to search should be A

						std::cout << "!! Returning line has a value of: ("
							<< searchResult.returnLine.line.pointA.x << ", "
							<< searchResult.returnLine.line.pointA.y << ", "
							<< searchResult.returnLine.line.pointA.z << ") | ("
							<< searchResult.returnLine.line.pointB.x << ", "
							<< searchResult.returnLine.line.pointB.y << ", "
							<< searchResult.returnLine.line.pointB.z << ") " << std::endl;
					}
					else if (pointCheckResult == IRPointType::POINT_A)
					{
						std::cout << "!!!! Entering point A logic..." << std::endl;
						searchResult.returnLine.line.pointA = in_pointToSearch;
						searchResult.nextPointToFind = searchResult.returnLine.line.pointB;	// next point to search should be B

						std::cout << "!! Returning line has a value of: ("
							<< searchResult.returnLine.line.pointA.x << ", "
							<< searchResult.returnLine.line.pointA.y << ", "
							<< searchResult.returnLine.line.pointA.z << ") | ("
							<< searchResult.returnLine.line.pointB.x << ", "
							<< searchResult.returnLine.line.pointB.y << ", "
							<< searchResult.returnLine.line.pointB.z << ") " << std::endl;
					}
					searchResult.wasFound = true;

					// remove the original line we altered from the pool
					int foundIndexAlternate = nonBoundSecondPassBegin->first;
					fetchAndRemoveLineAtIndex(IntersectionType::NON_BOUND, foundIndexAlternate); 

					// break out, as we are done.
					break;
				}
			}
		}

	}
	else
	{
		searchResult.wasFound = false;
	}

	return searchResult;
}

CategorizedLineSearchResult CategorizedLineManager::searchManagerForLastPartialBoundLineForSequence(glm::vec3 in_pointToSearch)
{
	CategorizedLineSearchResult searchResult;
	if (containerMap[IntersectionType::PARTIAL_BOUND].lineMap.size() > 0)	// search for PARTIAL_BOUND categorized lines, but only if there are ones to search for.
	{
		// search through all the partially bound lines.
		auto partialboundBegin = containerMap[IntersectionType::PARTIAL_BOUND].lineMap.begin();
		auto partialboundEnd = containerMap[IntersectionType::PARTIAL_BOUND].lineMap.end();
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

			//std::cout << "(CleaveSequenceFactory) One iteration passed through partial bounds..." << std::endl;
			categorizedLineManagerLogger.log("(CategorizedLineManager): One iteration passed through partial bounds...", "\n");
		}

		// if it was found, do this stuff (should be able to ignore invalid iterators this way)
		if (wasFound == true)
		{
			searchResult.wasFound = true;
			CategorizedLine foundLine = fetchAndRemoveLineAtIndex(IntersectionType::PARTIAL_BOUND, foundIndex);		// fetch and remove from the PARTIAL_BOUND pool, at the index.
			// remember, when dealing with the ending partial line, the matched point should be "point A" of the border line; if it isn't, we must swap the line.
			if (pointCheckResult == IRPointType::POINT_B)
			{
				foundLine.line.swapToA();	// it's backwards; so swap the lines.

			}
			searchResult.returnLine = foundLine;	// store the appropriate line
			searchResult.nextPointToFind = foundLine.line.pointB;	// set the next point to find.
		}

		// error/bad calculation handling; 9/6/2021 -- expand the point search "radius" by .2f 
		// if we find a line nearby, return a "modified" version of that line, and remove the original line
		// we based the altering off of from the corresponding pool.
		else if (wasFound == false)
		{
			std::cout << "!!! Notice -> wasFound flagged as false, running backup option..." << std::endl;
			std::cout << "pointToSearch is: " << in_pointToSearch.x << ", " << in_pointToSearch.y << ", " << in_pointToSearch.z << std::endl;
			searchResult.backupOptionFlag = true;

			auto partialBoundSecondPassBegin = containerMap[IntersectionType::PARTIAL_BOUND].lineMap.begin();
			auto partialBoundSecondPassEnd = containerMap[IntersectionType::PARTIAL_BOUND].lineMap.end();
			for (; partialBoundSecondPassBegin != partialBoundSecondPassEnd; partialBoundSecondPassBegin++)
			{
				pointCheckResult = partialBoundSecondPassBegin->second.checkIfPointIsNearbyPointInLine(in_pointToSearch, 0.02f);
				if (pointCheckResult != IRPointType::NEITHER) // it was found (it's either A or B)
				{
					//std::cout << "!!! NOTICE: found nearby point...continue? " << std::endl;
					//int nearbyFound = 3;
					//std::cin >> nearbyFound;

					std::cout << "(CategorizedLineManager::searchManagerForLastPartialBoundLineForSequence): NOTICE: found nearby point, continuing. " << std::endl;

					// because we found a nearby non-bound line, we must 
					// if we matched against point B of the non bound, then change point B of the line to be 
					searchResult.returnLine = partialBoundSecondPassBegin->second;	// insert the line...
					if (pointCheckResult == IRPointType::POINT_B)
					{
						searchResult.returnLine.line.pointB = in_pointToSearch;
						searchResult.nextPointToFind = searchResult.returnLine.line.pointA;	// next point to search should be A

						std::cout << "!! Returning line has a value of: ("
							<< searchResult.returnLine.line.pointA.x << ", "
							<< searchResult.returnLine.line.pointA.y << ", "
							<< searchResult.returnLine.line.pointA.z << ") | ("
							<< searchResult.returnLine.line.pointB.x << ", "
							<< searchResult.returnLine.line.pointB.y << ", "
							<< searchResult.returnLine.line.pointB.z << ") " << std::endl;
					}
					else if (pointCheckResult == IRPointType::POINT_A)
					{
						std::cout << "!!!! Entering point A logic..." << std::endl;
						searchResult.returnLine.line.pointA = in_pointToSearch;
						searchResult.nextPointToFind = searchResult.returnLine.line.pointB;	// next point to search should be B

						std::cout << "!! Returning line has a value of: ("
							<< searchResult.returnLine.line.pointA.x << ", "
							<< searchResult.returnLine.line.pointA.y << ", "
							<< searchResult.returnLine.line.pointA.z << ") | ("
							<< searchResult.returnLine.line.pointB.x << ", "
							<< searchResult.returnLine.line.pointB.y << ", "
							<< searchResult.returnLine.line.pointB.z << ") " << std::endl;
					}
					searchResult.wasFound = true;

					// remove the original line we altered from the pool
					int foundIndexAlternate = partialBoundSecondPassBegin->first;
					fetchAndRemoveLineAtIndex(IntersectionType::PARTIAL_BOUND, foundIndexAlternate);

					// break out, as we are done.
					break;
				}
			}
		}
	}
	else
	{
		searchResult.wasFound = false;
	}
	return searchResult;
}

CategorizedLineSearchResult CategorizedLineManager::searchManagerForInterceptPointPreciseCategorizedLine(glm::vec3 in_pointToSearch)
{
	CategorizedLineSearchResult searchResult;
	if (containerMap[IntersectionType::INTERCEPTS_POINT_PRECISE].lineMap.size() > 0)	// search for PARTIAL_BOUND categorized lines, but only if there are ones to search for.
	{
		// search through all the interceptPointPreciseLines.
		auto interceptPointPreciseBegin = containerMap[IntersectionType::INTERCEPTS_POINT_PRECISE].lineMap.begin();
		auto interceptPointPreciseEnd = containerMap[IntersectionType::INTERCEPTS_POINT_PRECISE].lineMap.end();
		bool wasFound = false;		// was a match found?
		int foundIndex = 0;
		IRPointType pointCheckResult = IRPointType::NEITHER;	// starts out as NEITHER
		for (interceptPointPreciseBegin; interceptPointPreciseBegin != interceptPointPreciseEnd; interceptPointPreciseBegin++)
		{
			//std::cout << "(CategorizedLineManager) !! Iterating through INTERCEPT_POINT_PRECISE map..." << std::endl;
			//std::cout << "(CategorizedLineManager) !! Line points are: " << std::endl;
			//std::cout << "(CategorizedLineManager) Point A: " << interceptPointPreciseBegin->second.line.pointA.x << ", " << interceptPointPreciseBegin->second.line.pointA.y << ", " << interceptPointPreciseBegin->second.line.pointA.z << std::endl;
			//std::cout << "(CategorizedLineManager) Point B: " << interceptPointPreciseBegin->second.line.pointB.x << ", " << interceptPointPreciseBegin->second.line.pointB.y << ", " << interceptPointPreciseBegin->second.line.pointB.z << std::endl;

			categorizedLineManagerLogger.log("(CategorizedLineManager): !! Iterating through INTERCEPT_POINT_PRECISE map...", "\n");
			categorizedLineManagerLogger.log("(CategorizedLineManager): !! Line points are: ", "\n");
			categorizedLineManagerLogger.log("(CategorizedLineManager): Point A: ", interceptPointPreciseBegin->second.line.pointA.x, ", ", interceptPointPreciseBegin->second.line.pointA.y, ", ", interceptPointPreciseBegin->second.line.pointA.z, "\n");
			categorizedLineManagerLogger.log("(CategorizedLineManager): Point B: ", interceptPointPreciseBegin->second.line.pointB.x, ", ", interceptPointPreciseBegin->second.line.pointB.y, ", ", interceptPointPreciseBegin->second.line.pointB.z, "\n");


			pointCheckResult = interceptPointPreciseBegin->second.checkIfPointIsInLine(in_pointToSearch);
			if (pointCheckResult != IRPointType::NEITHER)
			{
				wasFound = true;
				foundIndex = interceptPointPreciseBegin->first;
				//std::cout << "::::: found matching point to search, for a categorized line of type INTERCEPT_POINT_PRECISE! " << std::endl;
				break;

			}
		}

		// if it was found, do this stuff (should be able to ignore invalid iterators this way)
		if (wasFound == true)
		{

			searchResult.wasFound = true;
			//std::cout << "::::: set searchResult.wasFound as true..." << std::endl;
			CategorizedLine foundLine = fetchAndRemoveLineAtIndex(IntersectionType::INTERCEPTS_POINT_PRECISE, foundIndex);
			//CategorizedLine foundLine;

			// remember, when dealing with the ending partial line, the matched point should be "point A" of the border line; if it isn't, we must swap the line.
			if (pointCheckResult == IRPointType::POINT_B)
			{
				foundLine.line.swapToA();	// it's backwards; so swap the lines.

				//std::cout << "(Post-Alter) || ::: >> current categorized line, point A: " << foundLine.line.pointA.x << ", " << foundLine.line.pointA.y << ", " << foundLine.line.pointA.z << std::endl;
				//std::cout << "(Post-Alter) || ::: >> current categorized line, point A border: " << foundLine.line.pointABorder << std::endl;
				//std::cout << "(Post-Alter) || ::: >> current categorized line, is point A on border: " << foundLine.line.isPointAOnBorder << std::endl;

				//std::cout << "(Post-Alter) || ::: >> current categorized line, point B: " << foundLine.line.pointB.x << ", " << foundLine.line.pointB.y << ", " << foundLine.line.pointB.z << std::endl;
				//std::cout << "(Post-Alter) || ::: >> current categorized line, point B border: " << foundLine.line.pointBBorder << std::endl;
				//std::cout << "(Post-Alter) || ::: >> current categorized line, is point B on border: " << foundLine.line.isPointBOnBorder << std::endl;
			}

			searchResult.returnLine = foundLine;	// store the appropriate line
			searchResult.nextPointToFind = foundLine.line.pointB;	// set the next point to find.
		}

		// error/bad calculation handling; 9/6/2021 -- expand the point search "radius" by .2f 
		// if we find a line nearby, return a "modified" version of that line, and remove the original line
		// we based the altering off of from the corresponding pool.
		else if (wasFound == false)
		{
			std::cout << "!!! Notice -> wasFound flagged as false, running backup option..." << std::endl;
			std::cout << "pointToSearch is: " << in_pointToSearch.x << ", " << in_pointToSearch.y << ", " << in_pointToSearch.z << std::endl;
			searchResult.backupOptionFlag = true;

			auto preciseSecondPassBegin = containerMap[IntersectionType::INTERCEPTS_POINT_PRECISE].lineMap.begin();
			auto preciseSecondPassEnd = containerMap[IntersectionType::INTERCEPTS_POINT_PRECISE].lineMap.end();
			for (; preciseSecondPassBegin != preciseSecondPassEnd; preciseSecondPassBegin++)
			{
				pointCheckResult = preciseSecondPassBegin->second.checkIfPointIsNearbyPointInLine(in_pointToSearch, 0.02f);
				if (pointCheckResult != IRPointType::NEITHER) // it was found (it's either A or B)
				{
					//std::cout << "!!! NOTICE: found nearby point...continue? " << std::endl;
					//int nearbyFound = 3;
					//std::cin >> nearbyFound;

					std::cout << "(CategorizedLineManager::searchManagerForInterceptPointPreciseCategorizedLine): NOTICE: found nearby point, continuing. " << std::endl;

					// because we found a nearby non-bound line, we must 
					// if we matched against point B of the non bound, then change point B of the line to be 
					searchResult.returnLine = preciseSecondPassBegin->second;	// insert the line...
					if (pointCheckResult == IRPointType::POINT_B)
					{
						searchResult.returnLine.line.pointB = in_pointToSearch;
						searchResult.nextPointToFind = searchResult.returnLine.line.pointA;	// next point to search should be A

						std::cout << "!! Returning line has a value of: ("
							<< searchResult.returnLine.line.pointA.x << ", "
							<< searchResult.returnLine.line.pointA.y << ", "
							<< searchResult.returnLine.line.pointA.z << ") | ("
							<< searchResult.returnLine.line.pointB.x << ", "
							<< searchResult.returnLine.line.pointB.y << ", "
							<< searchResult.returnLine.line.pointB.z << ") " << std::endl;
					}
					else if (pointCheckResult == IRPointType::POINT_A)
					{
						std::cout << "!!!! Entering point A logic..." << std::endl;
						searchResult.returnLine.line.pointA = in_pointToSearch;
						searchResult.nextPointToFind = searchResult.returnLine.line.pointB;	// next point to search should be B

						std::cout << "!! Returning line has a value of: ("
							<< searchResult.returnLine.line.pointA.x << ", "
							<< searchResult.returnLine.line.pointA.y << ", "
							<< searchResult.returnLine.line.pointA.z << ") | ("
							<< searchResult.returnLine.line.pointB.x << ", "
							<< searchResult.returnLine.line.pointB.y << ", "
							<< searchResult.returnLine.line.pointB.z << ") " << std::endl;
					}
					searchResult.wasFound = true;

					// remove the original line we altered from the pool
					int foundIndexAlternate = preciseSecondPassBegin->first;
					fetchAndRemoveLineAtIndex(IntersectionType::INTERCEPTS_POINT_PRECISE, foundIndexAlternate);

					// break out, as we are done.
					break;
				}
			}
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