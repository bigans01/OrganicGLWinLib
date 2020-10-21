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

	CleaveSequenceMeta sequenceMeta(sequenceType, cleavingLines.size());
	return sequenceMeta;
}

glm::vec3 CleaveSequence::fetchPointToSearch()
{
	//glm::vec3 nonBorderPoint = cleavingLines[0].line.getNonBorderPointFromSingularBorderLineCount();
	return currentPointToSearch;
}