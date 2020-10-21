#pragma once

#ifndef CLEAVESEQUENCECANDIDATELISTMAP_H
#define CLEAVESEQUENCECANDIDATELISTMAP_H

#include <map>
#include "CleaveSequenceCandidateList.h"

class CleaveSequenceCandidateListMap
{
	public:
		std::map<int, CleaveSequenceCandidateList> candidateMap;
};

#endif
