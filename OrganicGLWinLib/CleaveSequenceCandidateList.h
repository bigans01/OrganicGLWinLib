#pragma once

#ifndef CLEAVESEQUENCECANDIDATELIST_H
#define CLEAVESEQUENCECANDIDATELIST_H

#include <set>
#include <iostream>

class CleaveSequenceCandidateList
{
	public:
		void insertCandidate(int in_candidateCleaveSequenceID)
		{
			candidateSet.insert(in_candidateCleaveSequenceID);
		}
		void removeCandidate(int in_candidateCleaveSequenceID)
		{
			candidateSet.erase(in_candidateCleaveSequenceID);
		}
		std::set<int> getCandidateSet()
		{
			return candidateSet;
		}
		void printCurrentCandidates()
		{
			auto setBegin = candidateSet.begin();
			auto setEnd = candidateSet.end();
			for (; setBegin != setEnd; setBegin++)
			{
				std::cout << *setBegin << std::endl;
			}
		}
	private:
		std::set<int> candidateSet;

	
};

#endif
