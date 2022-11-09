#pragma once

#ifndef CLEAVESEQUENCECANDIDATELISTMAP_H
#define CLEAVESEQUENCECANDIDATELISTMAP_H

#include "CleaveSequenceCandidateList.h"

class CleaveSequenceCandidateListMap
{
	public:
		std::map<int, CleaveSequenceCandidateList> candidateMap;
		int remainingCandidateCount = 0;
		void removeCandidateFromAllCandidateLists(int in_candidateCleaveSequenceID)
		{
			auto candidateListBegin = candidateMap.begin();
			auto candidateListEnd = candidateMap.end();
			for (; candidateListBegin != candidateListEnd; candidateListBegin++)
			{
				candidateListBegin->second.removeCandidate(in_candidateCleaveSequenceID);
			}
		}

		void printCandidateLists()
		{
			std::cout << ">>>>>>>>>>>> Printing candidate lists. " << std::endl;
			auto candidateListBegin = candidateMap.begin();
			auto candidateListEnd = candidateMap.end();
			for (; candidateListBegin != candidateListEnd; candidateListBegin++)
			{
				std::cout << ">>> Printing candidates for border line: " << candidateListBegin->first << std::endl;
				candidateListBegin->second.printCurrentCandidates();
			}
		}

		void updateRemainingCandidateCount()
		{
			remainingCandidateCount = 0;
			auto candidateListBegin = candidateMap.begin();
			auto candidateListEnd = candidateMap.end();
			for (; candidateListBegin != candidateListEnd; candidateListBegin++)
			{
				candidateListBegin->second.updateCounter(&remainingCandidateCount);
			}
			//std::cout << "!!! Remaining number of candidates: " << remainingCandidateCount << std::endl;
		};

		int fetchNextAvailableCandidate()
		{
			int returnCandidateID = 0;
			auto candidateListBegin = candidateMap.begin();
			auto candidateListEnd = candidateMap.end();
			for (; candidateListBegin != candidateListEnd; candidateListBegin++)
			{
				//candidateListBegin->second.updateCounter(&remainingCandidateCount);
				if (candidateListBegin->second.getCandidateSetSize() > 0)
				{
					returnCandidateID = candidateListBegin->second.getFirstAvailableCandidate();
					break;
				}
			}
			return returnCandidateID;
		}
};

#endif
