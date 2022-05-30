#include "stdafx.h"
#include "CleaveSequence.h"
#include "CategorizedLine.h"
#include <string>

void CleaveSequence::insertLineAtIndex(int in_index, CategorizedLine in_categorizedLine)
{
	CategorizedLine newLine = in_categorizedLine;	// copy
	newLine.cleaveSequenceID = in_index;			// insert what the ID in the sequence will be, into the line.
	cleavingLines[in_index] = newLine;
}

void CleaveSequence::insertFirstLine(CategorizedLine in_categorizedLine)
{
	CategorizedLine newLine = in_categorizedLine;
	newLine.cleaveSequenceID = cleaveIndex;
	cleavingLines[cleaveIndex] = newLine;
	cleaveIndex++;
	currentPointToSearch = cleavingLines[0].line.getNonBorderPointFromSingularBorderLineCount();
	//std::cout << "!! New line inserted into sequence: " << in_categorizedLine.line.pointA.x << ", " << in_categorizedLine.line.pointA.y << ", " << in_categorizedLine.line.pointA.z << " | " << in_categorizedLine.line.pointB.x << ", " << in_categorizedLine.line.pointB.y << ", " << in_categorizedLine.line.pointB.z << std::endl;
}

void CleaveSequence::insertNonboundLine(CategorizedLine in_categorizedLine)
{
	CategorizedLine newLine = in_categorizedLine;
	newLine.cleaveSequenceID = cleaveIndex;
	cleavingLines[cleaveIndex] = newLine;
	cleaveIndex++;
	currentPointToSearch = in_categorizedLine.line.pointB;
	
	//std::cout << ":::: non-bound line, point A: " << in_categorizedLine.line.pointA.x << ", " << in_categorizedLine.line.pointA.y << ", " << in_categorizedLine.line.pointA.z << std::endl;
	//std::cout << ":::: non-bound line, point B: " << in_categorizedLine.line.pointB.x << ", " << in_categorizedLine.line.pointB.y << ", " << in_categorizedLine.line.pointB.z << std::endl;

	//std::cout << ":: Inserted non-bound line; Next point to search is: " << currentPointToSearch.x << ", " << currentPointToSearch.y << ", " << currentPointToSearch.z << std::endl;
}

void CleaveSequence::insertLastLine(CategorizedLine in_categorizedLine)
{
	CategorizedLine newLine = in_categorizedLine;
	newLine.cleaveSequenceID = cleaveIndex;
	cleavingLines[cleaveIndex] = newLine;
	cleaveIndex++;
	//std::cout << "Final line inserted: " << in_categorizedLine.line.pointA.x << ", " << in_categorizedLine.line.pointA.y << ", " << in_categorizedLine.line.pointA.z << " | " << in_categorizedLine.line.pointB.x << ", " << in_categorizedLine.line.pointB.y << ", " << in_categorizedLine.line.pointB.z << std::endl;
}

int CleaveSequence::retrievedLineEndpointBorderLine(CleaveSequenceCrawlDirection in_crawlDirection)
{
	int returnBorderLineID = 0;
	if (in_crawlDirection == CleaveSequenceCrawlDirection::FORWARD)
	{
		auto crawlBegin = cleavingLines.rbegin();		// when going forward, the last line in the sequence to get the border line ID will be at rbegin.
		/*
		if (crawlBegin->second.type == IntersectionType::A_SLICE)
		{
			returnBorderLineID = crawlBegin->second.line.pointBBorder;
		}
		else if (crawlBegin->second.type == IntersectionType::PARTIAL_BOUND)
		{
			returnBorderLineID = crawlBegin->second.line.pointBBorder;
		}
		*/

		
		//std::cout << "::: >> current categorized line, point A: " << crawlBegin->second.line.pointA.x << ", " << crawlBegin->second.line.pointA.y << ", " << crawlBegin->second.line.pointA.z << std::endl;
		//std::cout << "::: >> current categorized line, point A border: " << crawlBegin->second.line.pointABorder << std::endl;
		//std::cout << "::: >> current categorized line, is point A on border: " << crawlBegin->second.line.isPointAOnBorder << std::endl;

		//std::cout << "::: >> current categorized line, point B: " << crawlBegin->second.line.pointB.x << ", " << crawlBegin->second.line.pointB.y << ", " << crawlBegin->second.line.pointB.z << std::endl;
		//std::cout << "::: >> current categorized line, point B border: " << crawlBegin->second.line.pointBBorder << std::endl;
		//std::cout << "::: >> current categorized line, is point B on border: " << crawlBegin->second.line.isPointBOnBorder << std::endl;

		// check that the line isn't a INTERCEPTS_POINT_PRECISE
		if (crawlBegin->second.type != IntersectionType::INTERCEPTS_POINT_PRECISE)
		{
			returnBorderLineID = crawlBegin->second.line.pointBBorder;
		}
		else if (crawlBegin->second.type == IntersectionType::INTERCEPTS_POINT_PRECISE)
		{
			//std::cout << "!!! Attempting border line retrieval from INTERCEPTS_POINT_PRECISE" << std::endl;
			returnBorderLineID = crawlBegin->second.line.getBorderLineIDFromSingularBorderLineCount();
		}

		//std::cout << "::::::::::: Return border line ID is: " << returnBorderLineID << std::endl;
	}
	else if (in_crawlDirection == CleaveSequenceCrawlDirection::REVERSE)
	{
		auto crawlBegin = cleavingLines.begin();		// when going reverse, the last line in the sequence to get the border line ID will be at begin.
		/*
		if (crawlBegin->second.type == IntersectionType::A_SLICE)
		{

		}
		else if (crawlBegin->second.type == IntersectionType::PARTIAL_BOUND)
		{

		}
		*/
		if (crawlBegin->second.type != IntersectionType::INTERCEPTS_POINT_PRECISE)
		{
			returnBorderLineID = crawlBegin->second.line.pointABorder;
		}
		else if (crawlBegin->second.type == IntersectionType::INTERCEPTS_POINT_PRECISE)
		{
			returnBorderLineID = crawlBegin->second.line.getBorderLineIDFromSingularBorderLineCount();
		}
	}
	return returnBorderLineID;
}

void CleaveSequence::printCategorizedLines()
{
	auto cleavingLinesBegin = cleavingLines.begin();
	auto cleavingLinesEnd = cleavingLines.end();
	for (; cleavingLinesBegin != cleavingLinesEnd; cleavingLinesBegin++)
	{
		
		std::cout << "IntersectionType of line: " << cleavingLinesBegin->second.getIntersectionTypeString() << " ";

		auto currentDirection = cleavingLinesBegin->second.direction;
		switch (currentDirection)
		{
			case CyclingDirection::NOVAL: { std::cout << "CyclingDirection: NOVAL "; break; }
			case CyclingDirection::FORWARD: { std::cout << "CyclingDirection: FORWARD "; break; }
			case CyclingDirection::REVERSE: { std::cout << "CyclingDirection: REVERSE "; break; }
		}

		std::cout << "[" << cleavingLinesBegin->first << "] | pointA: " << cleavingLinesBegin->second.line.pointA.x << ", " << cleavingLinesBegin->second.line.pointA.y << ", " << cleavingLinesBegin->second.line.pointA.z
													<< " | pointB: " << cleavingLinesBegin->second.line.pointB.x << ", " << cleavingLinesBegin->second.line.pointB.y << ", " << cleavingLinesBegin->second.line.pointB.z 
													<< " | A border: (is on border)-> " << cleavingLinesBegin->second.line.isPointAOnBorder << ", (border value)-> " << cleavingLinesBegin->second.line.pointABorder
													<< " | B border: (is on border)-> " << cleavingLinesBegin->second.line.isPointBOnBorder << ", (border value)-> " << cleavingLinesBegin->second.line.pointBBorder
													<< " | parent poly: " << cleavingLinesBegin->second.parentPoly
													<< std::endl;
		std::cout << "Number of border lines: " << cleavingLinesBegin->second.line.numberOfBorderLines << std::endl;

		
	}
}

void CleaveSequence::setSequenceAsSuper()
{
	hierarchyPosition = LineWelderHierarchyPosition::SUPER;
}

CategorizedLine* CleaveSequence::getCategorizedLineRef(int in_categorizedLineID)
{
	return &cleavingLines[in_categorizedLineID];
}

CleaveSequenceMeta CleaveSequence::getCleaveSequenceMeta()
{
	CleaveSequenceType sequenceType = CleaveSequenceType::UNDEFINED;	// it will be defined in next lines
	if (cleavingLines.size() == 1)	// if it's just one line, do this
	{
		sequenceType = CleaveSequenceType::SINGLE_LINE;
	}
	else if (cleavingLines.size() >= 1)	// multiple lines
	{
		sequenceType = CleaveSequenceType::MULTI_LINE;
	}

	CleaveSequenceMeta sequenceMeta(sequenceType, int(cleavingLines.size()), this);
	return sequenceMeta;
}

glm::vec3 CleaveSequence::fetchPointToSearch()
{
	//glm::vec3 nonBorderPoint = cleavingLines[0].line.getNonBorderPointFromSingularBorderLineCount();
	return currentPointToSearch;
}

CategorizedLine CleaveSequence::fetchCategorizedLineFromSequenceThatHasBorderPoint(glm::vec3 in_borderPoint, PolyDebugLevel in_polyDebugLevel)
{
	CategorizedLine returnLine;
	PolyLogger tempLogger;
	tempLogger.setDebugLevel(in_polyDebugLevel);
	tempLogger.log("(CleaveSequence)  ---Fetching categorized line that has border point of:", in_borderPoint.x, ",", in_borderPoint.y, ", ", in_borderPoint.z, "\n");
	if (cleavingLines.size() == 1)
	{
		auto categorizedLineIter = cleavingLines.begin();
		returnLine = categorizedLineIter->second;
		if (in_borderPoint != returnLine.line.pointA)
		{
			returnLine.line.swapToA();
		}
	}
	else if (cleavingLines.size() > 1)
	{
		auto firstLine = cleavingLines.begin();
		auto lastLine = cleavingLines.rbegin();

		// first line analytics
		bool firstLineMatched = false;
		auto firstLineCopy = firstLine->second;
		IRPointType firstLineType = firstLineCopy.checkIfPointIsInLine(in_borderPoint);
		if (firstLineType != IRPointType::NEITHER)	// it was found
		{
			firstLineMatched = true;
			// if it was matched, but the point wasn't point A, swap to A.
			if (firstLineType == IRPointType::POINT_B)
			{
				firstLineCopy.line.swapToA();
			}
		}

		// second line analytics
		bool lastLineMatched = false;
		auto lastLineCopy = lastLine->second;
		IRPointType lastLineType = lastLineCopy.checkIfPointIsInLine(in_borderPoint);
		if (lastLineType != IRPointType::NEITHER)
		{
			lastLineMatched = true;
			// if it was matched, but the point wasn't point A, swap to A.
			if (lastLineType == IRPointType::POINT_B)
			{
				lastLineCopy.line.swapToA();
			}
		}


		// get the line based on the analytics.
		if (firstLineMatched == true)
		{
			returnLine = firstLineCopy;
		}
		else if (lastLineMatched == true)
		{
			returnLine = lastLineCopy;
		}
	}
	return returnLine;
}

DistanceToPoint CleaveSequence::fetchClosestPointOnBorderLineID(glm::vec3 in_pointToCalculateFor, int in_borderLineID, PolyDebugLevel in_polyDebugLevel)
{
	PolyLogger tempLogger;
	tempLogger.setDebugLevel(in_polyDebugLevel);
	tempLogger.log("(CleaveSequence)  ---Fetching closest point, typical---", "\n");

	DistanceToPoint returnDistanceToPoint;
	float currentShortest = 100000.0f;
	if (cleavingLines.size() == 1)
	{
		auto categorizedLineIter = cleavingLines.begin();
		glm::vec3 pointA = categorizedLineIter->second.line.pointA;
		glm::vec3 pointB = categorizedLineIter->second.line.pointB;

		float distOriginToA = glm::distance(in_pointToCalculateFor, pointA);
		float distOriginToB = glm::distance(in_pointToCalculateFor, pointB);
		float selectedMin = std::min(distOriginToA, distOriginToB);			// see
																				// https://stackoverflow.com/questions/5004858/why-is-stdmin-failing-when-windows-h-is-included

		// is the chosen min for A?
		if (selectedMin == distOriginToA)
		{
			//std::cout << "(SINGLE_LINE) Closest point is A" << std::endl;
			returnDistanceToPoint.distance = selectedMin;
			returnDistanceToPoint.point = pointA;
		}

		// is the chosen min for B?
		else if (selectedMin == distOriginToB)
		{
			//std::cout << "(SINGLE_LINE) Closest point is B" << std::endl;
			returnDistanceToPoint.distance = selectedMin;
			returnDistanceToPoint.point = pointB;
		}

		// special case, when they are equal
		else if (distOriginToA == distOriginToB)
		{
			//std::cout << "!!! Warning, EQUAL distance found...needs handling..." << std::endl;
		}

	}
	else if (cleavingLines.size() >= 1)
	{
		// remember, if there are two or more cleaving lines, the first and last should contain border points.
	
		//std::cout << "!!! Size of cleave map is greater than one; executing this branch...." << std::endl;
		//std::cout << "!!! Point to calculate for is: " << in_pointToCalculateFor.x << ", " << in_pointToCalculateFor.y << ", " << in_pointToCalculateFor.z << std::endl;
		//std::cout << "!!! Border Line ID to match for: " << in_borderLineID << std::endl;

		//tempLogger.log("(CleaveSequence)  ---Fetching closest point, typical---", "\n");
		tempLogger.log("(CleaveSequence) !!! Size of cleave map is greater than one; executing this branch....", "\n");
		tempLogger.log("(CleaveSequence) !!! Point to calculate for is: ", in_pointToCalculateFor.x, ", ", in_pointToCalculateFor.y, ", ", in_pointToCalculateFor.z, "\n");
		tempLogger.log("(CleaveSequence) !!! Border Line ID to match for: ", in_borderLineID, "\n");

		auto firstLine = cleavingLines.begin();
		auto lastLine = cleavingLines.rbegin();

		//std::cout << "First Line; isAOnBorder: " << firstLine->second.line.isPointAOnBorder << ", " << firstLine->second.line.pointABorder << std::endl;
		//std::cout << "Last Line; isBOnBorder: " << lastLine->second.line.isPointBOnBorder << ", " << lastLine->second.line.pointBBorder << std::endl;

		tempLogger.log("(CleaveSequence) First Line; isAOnBorder: ", firstLine->second.line.isPointAOnBorder, ", ", firstLine->second.line.pointABorder, "\n");
		tempLogger.log("(CleaveSequence) Last line; isBOnBorder: ", lastLine->second.line.isPointBOnBorder, ", ", lastLine->second.line.pointBBorder, "\n");

		glm::vec3 pointA = firstLine->second.line.getBorderPointFromSingularBorderLineCount();
		glm::vec3 pointB = lastLine->second.line.getBorderPointFromSingularBorderLineCount();

		float distOriginToA = glm::distance(in_pointToCalculateFor, pointA);
		float distOriginToB = glm::distance(in_pointToCalculateFor, pointB);

		//std::cout << "Point A: " << pointA.x << ", " << pointA.y << ", " << pointA.z << " | Distance: " << distOriginToA << std::endl;
		//std::cout << "Point B: " << pointB.x << ", " << pointB.y << ", " << pointB.z << " | Distance: " << distOriginToB << std::endl;

		tempLogger.log("(CleaveSequence) Point A: ", pointA.x, ", ", pointA.y, ", ", pointA.z, " | Distance: ", distOriginToA, "\n");
		tempLogger.log("(CleaveSequence) Point B: ", pointB.x, ", ", pointB.y, ", ", pointB.z, " | Distance: ", distOriginToB, "\n");


		float selectedMin = std::min(distOriginToA, distOriginToB);		// see line 168 for URL (above)
																				
		/*
		// is the chosen min for A?
		if (selectedMin == distOriginToA)
		{
			//std::cout << "(MULTI_LINE) Closest point is A" << std::endl;
			returnDistanceToPoint.distance = selectedMin;
			returnDistanceToPoint.point = pointA;
		}

		// is the chosen min for B?
		else if (selectedMin == distOriginToB)
		{
			//std::cout << "(MULTI_LINE) Closest point is B" << std::endl;
			returnDistanceToPoint.distance = selectedMin;
			returnDistanceToPoint.point = pointB;
		}

		// special case, when they are equal
		else if (distOriginToA == distOriginToB)
		{
			//std::cout << "!!! Warning, EQUAL distance found...needs handling..." << std::endl;
		}
		*/

		// ****************************** NEW CODE (1/22/2021)
		auto newFirstLine = cleavingLines.begin();
		auto newLastLine = cleavingLines.rbegin();

		BorderLineData firstLineData = newFirstLine->second.line.getBorderLineDataForSingularBorderLineCount();
		BorderLineData lastLineData = newLastLine->second.line.getBorderLineDataForSingularBorderLineCount();

		std::vector<BorderLineData> dataVector;
		if (firstLineData.borderLineID == in_borderLineID)
		{
			//std::cout << "!!! Point A (first line) inserted into data vector. " << std::endl;
			tempLogger.log("(CleaveSequence) !!! Point A (first line) inserted into data vector. ", "\n");
			dataVector.push_back(firstLineData);
		}
		if (lastLineData.borderLineID == in_borderLineID)
		{
			//std::cout << "!!! Point B (last line) inserted into data vector. " << std::endl;
			tempLogger.log("(CleaveSequence) !!! Point B (last line) inserted into data vector. ", "\n");
			dataVector.push_back(lastLineData);
		}

		// now, check the vector size.
		if (dataVector.size() == 1)
		{
			auto dataVectorBegin = dataVector.begin();
			returnDistanceToPoint.distance = glm::distance(in_pointToCalculateFor, dataVectorBegin->point);
			returnDistanceToPoint.point = dataVectorBegin->point;
		}
		else if (dataVector.size() == 2)
		{
			auto dataVectorBegin = dataVector.begin();
			auto dataVectorEnd = dataVector.rbegin();

			glm::vec3 pointA = dataVectorBegin->point;
			glm::vec3 pointB = dataVectorEnd->point;

			float distOriginToA = glm::distance(in_pointToCalculateFor, pointA);
			float distOriginToB = glm::distance(in_pointToCalculateFor, pointB);

			float selectedMin = std::min(distOriginToA, distOriginToB);		// see line 168 for URL (above)

			// is the chosen min for A?
			if (selectedMin == distOriginToA)
			{
				//std::cout << "(MULTI_LINE) Closest point is A" << std::endl;
				returnDistanceToPoint.distance = selectedMin;
				returnDistanceToPoint.point = pointA;
			}

			// is the chosen min for B?
			else if (selectedMin == distOriginToB)
			{
				//std::cout << "(MULTI_LINE) Closest point is B" << std::endl;
				returnDistanceToPoint.distance = selectedMin;
				returnDistanceToPoint.point = pointB;
			}
		}


	}
	//std::cout << "+++++++++++++++ (Typical) Fetch closest point halt. " << std::endl;
	//int someVal = 5;
	//std::cin >> someVal;

	return returnDistanceToPoint;
}

DistanceToPoint CleaveSequence::fetchClosestPointSelfCompare(glm::vec3 in_pointToCalculateFor, PolyDebugLevel in_polyDebugLevel)
{
	PolyLogger tempLogger;
	tempLogger.setDebugLevel(in_polyDebugLevel);
	tempLogger.log("(CleaveSequence)  ---Fetching closest point, self-compare---", "\n");

	DistanceToPoint returnDistanceToPoint;
	float currentShortest = 100000.0f;
	if (cleavingLines.size() == 1)
	{
		auto categorizedLineIter = cleavingLines.begin();
		glm::vec3 pointA = categorizedLineIter->second.line.pointA;
		glm::vec3 pointB = categorizedLineIter->second.line.pointB;

		float distOriginToA = glm::distance(in_pointToCalculateFor, pointA);
		float distOriginToB = glm::distance(in_pointToCalculateFor, pointB);
		float selectedMin = std::min(distOriginToA, distOriginToB);		//  see line 168 for URL (above)

		// is the chosen min for A?
		if (selectedMin == distOriginToA)
		{
			//std::cout << "(SINGLE_LINE) Closest point is A" << std::endl;
			returnDistanceToPoint.distance = selectedMin;
			returnDistanceToPoint.point = pointA;
		}

		// is the chosen min for B?
		else if (selectedMin == distOriginToB)
		{
			//std::cout << "(SINGLE_LINE) Closest point is B" << std::endl;
			returnDistanceToPoint.distance = selectedMin;
			returnDistanceToPoint.point = pointB;
		}

		// special case, when they are equal
		else if (distOriginToA == distOriginToB)
		{
			//std::cout << "!!! Warning, EQUAL distance found...needs handling..." << std::endl;
		}

	}
	else if (cleavingLines.size() >= 1)
	{
		// remember, if there are two or more cleaving lines, the first and last should contain border points.
		auto firstLine = cleavingLines.begin();
		auto lastLine = cleavingLines.rbegin();

		glm::vec3 pointA = firstLine->second.line.getBorderPointFromSingularBorderLineCount();
		glm::vec3 pointB = lastLine->second.line.getBorderPointFromSingularBorderLineCount();

		float distOriginToA = glm::distance(in_pointToCalculateFor, pointA);
		float distOriginToB = glm::distance(in_pointToCalculateFor, pointB);

		//std::cout << "Point A: " << pointA.x << ", " << pointA.y << ", " << pointA.z << " | Distance: " << distOriginToA << std::endl;
		//std::cout << "Point B: " << pointB.x << ", " << pointB.y << ", " << pointB.z << " | Distance: " << distOriginToB << std::endl;
		tempLogger.log("(CleaveSequence) Point A: ", pointA.x, ", ", pointA.y, ",  ", pointA.z, " | Distance: ", distOriginToA, "\n");
		tempLogger.log("(CleaveSequence) Point B: ", pointB.x, ", ", pointB.y, ",  ", pointB.z, " | Distance: ", distOriginToB, "\n");


		float selectedMin = std::min(distOriginToA, distOriginToB);			// see line 168 for URL (above)

		// handle case 1: distToOriginA or distToOriginB is equal to 0.
		if
		(
			(distOriginToA == 0.0f)
			||
			(distOriginToB == 0.0f)
		)
		{
			//std::cout << "++++++++ fetchClosestPoint, distance logic check branch 1 entered..." << std::endl;
			tempLogger.log("(CleaveSequence) ++++++++ fetchClosestPoint, distance logic check branch 1 entered...", "\n");

			if (distOriginToA != 0.0f)
			{
				//std::cout << "(MULTI_LINE) Closest point is A" << std::endl;
				tempLogger.log("(CleaveSequence) (MULTI_LINE) Closest point is A", "\n");
				returnDistanceToPoint.distance = distOriginToA;
				returnDistanceToPoint.point = pointA;
			}
			else
			{
				//std::cout << "(MULTI_LINE) Closest point is B" << std::endl;
				tempLogger.log("(CleaveSequence) (MULTI_LINE) Closest point is B", "\n");
				returnDistanceToPoint.distance = distOriginToB;
				returnDistanceToPoint.point = pointB;
			}
			
		}

		// handle case 2: neither have a distance of 0; we just need to get the minimum
		else
		{
			// is the chosen min for A?
			if
				(
				(selectedMin == distOriginToA)
					//&&
					//(distOriginToA != 0.0f)
					)
			{
				//std::cout << "(MULTI_LINE) Closest point is A" << std::endl;
				tempLogger.log("(CleaveSequence) (MULTI_LINE) Closest point is A", "\n");
				returnDistanceToPoint.distance = selectedMin;
				returnDistanceToPoint.point = pointA;
			}

			// is the chosen min for B?
			else if
				(
				(selectedMin == distOriginToB)
					//&&
					//(distOriginToB != 0.0f)
					)
			{
				//std::cout << "(MULTI_LINE) Closest point is B" << std::endl;
				tempLogger.log("(CleaveSequence) (MULTI_LINE) Closest point is B", "\n");
				returnDistanceToPoint.distance = selectedMin;
				returnDistanceToPoint.point = pointB;
			}

			// special case, when they are equal
			else if (distOriginToA == distOriginToB)
			{
				//std::cout << "!!! Warning, EQUAL distance found...needs handling..." << std::endl;
			}
		}
	}

	//std::cout << "+++++++++++++++ (Self-compare) Fetch closest point halt. " << std::endl;
	//int someVal = 5;
	//std::cin >> someVal;

	tempLogger.log("(CleaveSequence) +++++++++++++++ (Self-compare) Fetch closest point halt; waiting for numerical input. ", "\n");
	tempLogger.waitForDebugInput();

	return returnDistanceToPoint;
}