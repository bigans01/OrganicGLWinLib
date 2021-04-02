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

void CategorizedLineManager::determineCyclingDirections(std::map<int, SPolyBorderLines> in_borderLineArrayRef, PolyDebugLevel in_polyDebugLevel)
{
	auto containerMapBegin = containerMap.begin();
	auto containerMapEnd = containerMap.end();
	for (; containerMapBegin != containerMapEnd; containerMapBegin++)
	{
		auto currentLineBegin = containerMapBegin->second.lineMap.begin();
		auto currentLineEnd = containerMapBegin->second.lineMap.end();
		for (; currentLineBegin != currentLineEnd; currentLineBegin++)
		{
			currentLineBegin->second.determineCyclingDirection(in_borderLineArrayRef, in_polyDebugLevel);
		}
	}
}

CategorizedLineSearchResult CategorizedLineManager::checkManagerForNextNonboundLine(glm::vec3 in_pointToSearch)
{
	categorizedLineManagerLogger.log("(CategorizedLineManager): Checking for next non-bound line, that has this point: ", in_pointToSearch.x, ",", in_pointToSearch.y, ", ", in_pointToSearch.z, "\n");
	CategorizedLineSearchResult searchResult;
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
			categorizedLineManagerLogger.log("(CategorizedLineManager): Checking non bound line. ", "\n");
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