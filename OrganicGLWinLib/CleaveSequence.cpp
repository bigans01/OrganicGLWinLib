#include "stdafx.h"
#include "CleaveSequence.h"

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
	
	std::cout << ":::: non-bound line, point A: " << in_categorizedLine.line.pointA.x << ", " << in_categorizedLine.line.pointA.y << ", " << in_categorizedLine.line.pointA.z << std::endl;
	std::cout << ":::: non-bound line, point B: " << in_categorizedLine.line.pointB.x << ", " << in_categorizedLine.line.pointB.y << ", " << in_categorizedLine.line.pointB.z << std::endl;

	std::cout << ":: Inserted non-bound line; Next point to search is: " << currentPointToSearch.x << ", " << currentPointToSearch.y << ", " << currentPointToSearch.z << std::endl;
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
		returnBorderLineID = crawlBegin->second.line.pointBBorder;
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
		returnBorderLineID = crawlBegin->second.line.pointABorder;
	}
	return returnBorderLineID;
}

void CleaveSequence::printCategorizedLines()
{
	auto cleavingLinesBegin = cleavingLines.begin();
	auto cleavingLinesEnd = cleavingLines.end();
	for (; cleavingLinesBegin != cleavingLinesEnd; cleavingLinesBegin++)
	{
		std::cout << "[" << cleavingLinesBegin->first << "] | pointA: " << cleavingLinesBegin->second.line.pointA.x << ", " << cleavingLinesBegin->second.line.pointA.y << ", " << cleavingLinesBegin->second.line.pointA.z
													<< " | pointB: " << cleavingLinesBegin->second.line.pointB.x << ", " << cleavingLinesBegin->second.line.pointB.y << ", " << cleavingLinesBegin->second.line.pointB.z 
													<< " | A border: (is on border)-> " << cleavingLinesBegin->second.line.isPointAOnBorder << ", (border value)-> " << cleavingLinesBegin->second.line.pointABorder
													<< " | B border: (is on border)-> " << cleavingLinesBegin->second.line.isPointBOnBorder << ", (border value)-> " << cleavingLinesBegin->second.line.pointBBorder
													<< std::endl;
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

	CleaveSequenceMeta sequenceMeta(sequenceType, cleavingLines.size(), this);
	return sequenceMeta;
}

glm::vec3 CleaveSequence::fetchPointToSearch()
{
	//glm::vec3 nonBorderPoint = cleavingLines[0].line.getNonBorderPointFromSingularBorderLineCount();
	return currentPointToSearch;
}

DistanceToPoint CleaveSequence::fetchClosestPoint(glm::vec3 in_pointToCalculateFor)
{
	DistanceToPoint returnDistanceToPoint;
	float currentShortest = 100000.0f;
	if (cleavingLines.size() == 1)
	{
		auto categorizedLineIter = cleavingLines.begin();
		glm::vec3 pointA = categorizedLineIter->second.line.pointA;
		glm::vec3 pointB = categorizedLineIter->second.line.pointB;

		float distOriginToA = glm::distance(in_pointToCalculateFor, pointA);
		float distOriginToB = glm::distance(in_pointToCalculateFor, pointB);
		float selectedMin = std::min(distOriginToA, distOriginToB);

		// is the chosen min for A?
		if (selectedMin == distOriginToA)
		{
			std::cout << "(SINGLE_LINE) Closest point is A" << std::endl;
			returnDistanceToPoint.distance = selectedMin;
			returnDistanceToPoint.point = pointA;
		}

		// is the chosen min for B?
		else if (selectedMin == distOriginToB)
		{
			std::cout << "(SINGLE_LINE) Closest point is B" << std::endl;
			returnDistanceToPoint.distance = selectedMin;
			returnDistanceToPoint.point = pointB;
		}

		// special case, when they are equal
		else if (distOriginToA == distOriginToB)
		{
			std::cout << "!!! Warning, EQUAL distance found...needs handling..." << std::endl;
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

		std::cout << "Point A: " << pointA.x << ", " << pointA.y << ", " << pointA.z << " | Distance: " << distOriginToA << std::endl;
		std::cout << "Point B: " << pointB.x << ", " << pointB.y << ", " << pointB.z << " | Distance: " << distOriginToB << std::endl;


		float selectedMin = std::min(distOriginToA, distOriginToB);

		// is the chosen min for A?
		if (selectedMin == distOriginToA)
		{
			std::cout << "(MULTI_LINE) Closest point is A" << std::endl;
			returnDistanceToPoint.distance = selectedMin;
			returnDistanceToPoint.point = pointA;
		}

		// is the chosen min for B?
		else if (selectedMin == distOriginToB)
		{
			std::cout << "(MULTI_LINE) Closest point is B" << std::endl;
			returnDistanceToPoint.distance = selectedMin;
			returnDistanceToPoint.point = pointB;
		}

		// special case, when they are equal
		else if (distOriginToA == distOriginToB)
		{
			std::cout << "!!! Warning, EQUAL distance found...needs handling..." << std::endl;
		}

	}
	return returnDistanceToPoint;
}