#include "stdafx.h"
#include "CleaveSequence.h"

void CleaveSequence::insertFirstLine(CategorizedLine in_categorizedLine)
{
	cleavingLines[cleaveIndex++] = in_categorizedLine;
	currentPointToSearch = cleavingLines[0].line.getNonBorderPointFromSingularBorderLineCount();
	//std::cout << "!! New line inserted into sequence: " << in_categorizedLine.line.pointA.x << ", " << in_categorizedLine.line.pointA.y << ", " << in_categorizedLine.line.pointA.z << " | " << in_categorizedLine.line.pointB.x << ", " << in_categorizedLine.line.pointB.y << ", " << in_categorizedLine.line.pointB.z << std::endl;
}

void CleaveSequence::insertNonboundLine(CategorizedLine in_categorizedLine)
{
	cleavingLines[cleaveIndex++] = in_categorizedLine;
	currentPointToSearch = in_categorizedLine.line.pointB;
	//std::cout << "Next point to search is: " << currentPointToSearch.x << ", " << currentPointToSearch.y << ", " << currentPointToSearch.z << std::endl;
}

void CleaveSequence::insertLastLine(CategorizedLine in_categorizedLine)
{
	cleavingLines[cleaveIndex++] = in_categorizedLine;
	//std::cout << "Final line inserted: " << in_categorizedLine.line.pointA.x << ", " << in_categorizedLine.line.pointA.y << ", " << in_categorizedLine.line.pointA.z << " | " << in_categorizedLine.line.pointB.x << ", " << in_categorizedLine.line.pointB.y << ", " << in_categorizedLine.line.pointB.z << std::endl;
}

glm::vec3 CleaveSequence::fetchPointToSearch()
{
	//glm::vec3 nonBorderPoint = cleavingLines[0].line.getNonBorderPointFromSingularBorderLineCount();
	return currentPointToSearch;
}