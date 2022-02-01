#pragma once

#ifndef INVALIDCLEAVESEQUENCES_H
#define INVALIDCLEAVESEQUENCES_H

#include <map>
#include "CleaveSequence.h"
#include <iostream>

class InvalidCleaveSequences
{
	public:	
		void insertCleaveSequenceAtKey(int in_keyValue, CleaveSequence in_cleaveSequence)
		{
			containsSequnces = true;
			sequenceMap[in_keyValue] = in_cleaveSequence;
		}
		void printInvalidSequenceData()
		{
			std::cout << "(InvalidCleaveSequences): printing invalid sequence metadata... " << std::endl;
			auto invalidsBegin = sequenceMap.begin();
			auto invalidsEnd = sequenceMap.end();
			for (; invalidsBegin != invalidsEnd; invalidsBegin++)
			{
				std::cout << "->Invalid ID: " << invalidsBegin->first << std::endl;
				invalidsBegin->second.printCategorizedLines();
			}
		}
		bool containsSequnces = false;
		std::map<int, CleaveSequence> sequenceMap;

};

#endif
