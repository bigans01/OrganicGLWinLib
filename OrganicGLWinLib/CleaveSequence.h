#pragma once

#ifndef CLEAVESEQUENCE_H
#define CLEAVESEQUENCE_H

#include "CleaveSequenceStatus.h"
#include "CleaveSequenceMeta.h"
#include "DistanceToPoint.h"
#include "LineWelderHierarchyPosition.h"
#include "CleaveSequenceCrawlDirection.h"
#include "BorderLineData.h"
#include "PolyLogger.h"
#include "PolyDebugLevel.h"

class CategorizedLine;
class CleaveSequence
{
public:
	std::map<int, CategorizedLine> cleavingLines;
	int cleaveIndex = 0;
	glm::vec3 currentPointToSearch;
	CleaveSequenceStatus sequenceStatus = CleaveSequenceStatus::NOVAL;		// default is noval (make compiler happy)
	LineWelderHierarchyPosition hierarchyPosition = LineWelderHierarchyPosition::SUB;	// by default, all lines will be a SUB; but at least one will get set to SUPER
																						// during the LineWelder's welding process.
	bool specialLogicUsedFlag = false;	// used to indicate that backup options were used one of the CategorizedLineManager functions.

	void insertLineAtIndex(int in_index, CategorizedLine in_categorizedLine);
	void insertFirstLine(CategorizedLine in_categorizedLine);
	void insertNonboundLine(CategorizedLine in_categorizedLine);
	void insertLastLine(CategorizedLine in_categorizedLine);
	int retrievedLineEndpointBorderLine(CleaveSequenceCrawlDirection in_crawlDirection);
	void printCategorizedLines();
	void setSequenceAsSuper();
	CategorizedLine* getCategorizedLineRef(int in_categorizedLineID);
	CleaveSequenceMeta getCleaveSequenceMeta();
	glm::vec3 fetchPointToSearch();
	DistanceToPoint fetchClosestPointOnBorderLineID(glm::vec3 in_pointToCalculateFor, int in_borderLineID, PolyDebugLevel in_polyDebugLevel);
	DistanceToPoint fetchClosestPointSelfCompare(glm::vec3 in_pointToCalculateFor, PolyDebugLevel in_polyDebugLevel);
	CategorizedLine fetchCategorizedLineFromSequenceThatHasBorderPoint(glm::vec3 in_borderPoint, PolyDebugLevel in_polyDebugLevel);
};

#endif