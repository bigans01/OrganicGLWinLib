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
		void updateCounter(int* in_counterRef)
		{
			auto setBegin = candidateSet.begin();
			auto setEnd = candidateSet.end();
			for (; setBegin != setEnd; setBegin++)
			{
				(*in_counterRef)++;
			}
		}
		int getCandidateSetSize()
		{
			return int(candidateSet.size());
		}
		int getFirstAvailableCandidate()
		{
			return *candidateSet.begin();
		}
	private:
		std::set<int> candidateSet;

	
};

#endif
