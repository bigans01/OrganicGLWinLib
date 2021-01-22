#pragma once

#ifndef CLEAVESEQUENCE_H
#define CLEAVESEQUENCE_H

//#include "CategorizedLine.h"
#include "CleaveSequenceStatus.h"
#include <map>
#include "CleaveSequenceMeta.h"
#include "DistanceToPoint.h"
#include <algorithm>
#include "LineWelderHierarchyPosition.h"
#include "CleaveSequenceCrawlDirection.h"
#include "BorderLineData.h"

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
	DistanceToPoint fetchClosestPointOnBorderLineID(glm::vec3 in_pointToCalculateFor, int in_borderLineID);
	DistanceToPoint fetchClosestPointSelfCompare(glm::vec3 in_pointToCalculateFor);
};

#endif