#pragma once

#ifndef CLEAVESEQUENCE_H
#define CLEAVESEQUENCE_H

#include "CategorizedLine.h"
#include <map>

class CleaveSequence
{
public:
	std::map<int, CategorizedLine> cleavingLines;
	int cleaveIndex = 0;
	glm::vec3 currentPointToSearch;

	void insertFirstLine(CategorizedLine in_categorizedLine);
	void insertNonboundLine(CategorizedLine in_categorizedLine);
	glm::vec3 fetchPointToSearch();
};

#endif