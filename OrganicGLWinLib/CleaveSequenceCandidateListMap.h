#pragma once

#ifndef CLEAVESEQUENCECANDIDATELISTMAP_H
#define CLEAVESEQUENCECANDIDATELISTMAP_H

#include <map>
#include "CleaveSequenceCandidateList.h"

class CleaveSequenceCandidateListMap
{
	public:
		std::map<int, CleaveSequenceCandidateList> candidateMap;
		void removeCandidateFromAllCandidateLists(int in_candidateCleaveSequenceID)
		{
			auto candidateListBegin = candidateMap.begin();
			auto candidateListEnd = candidateMap.end();
			for (; candidateListBegin != candidateListEnd; candidateListBegin++)
			{
				candidateListBegin->second.removeCandidate(in_candidateCleaveSequenceID);
			}
		}
};

#endif
