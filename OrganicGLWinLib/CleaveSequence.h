#pragma once

#ifndef CLEAVESEQUENCE_H
#define CLEAVESEQUENCE_H

#include "CategorizedLine.h"
#include "CleaveSequenceStatus.h"
#include <map>
#include "CleaveSequenceMeta.h"

class CleaveSequence
{
public:
	std::map<int, CategorizedLine> cleavingLines;
	int cleaveIndex = 0;
	glm::vec3 currentPointToSearch;
	CleaveSequenceStatus sequenceStatus = CleaveSequenceStatus::NOVAL;		// default is noval (make compiler happy)

	void insertFirstLine(CategorizedLine in_categorizedLine);
	void insertNonboundLine(CategorizedLine in_categorizedLine);
	void insertLastLine(CategorizedLine in_categorizedLine);
	CleaveSequenceMeta getCleaveSequenceMeta();
	glm::vec3 fetchPointToSearch();
};

#endif