#pragma once

#ifndef FUSEDPOINTMETAPAIR_H
#define FUSEDPOINTMETAPAIR_H

#include <map>
#include "FusedPointMeta.h"

class FusedPointMetaPair
{
	public:
		void insertFusedPointMeta(FusedPointMeta in_fusedPointMeta)
		{
			int currentIndex = metaPairMap.size();
			metaPairMap[currentIndex] = in_fusedPointMeta;
		}
		void printSummaries()
		{
			auto metaPairMapBegin = metaPairMap.begin();
			auto metaPairMapEnd = metaPairMap.end();
			for (; metaPairMapBegin != metaPairMapEnd; metaPairMapBegin++)
			{
				std::cout << "Meta summary for point at index " << metaPairMapBegin->first  << " ";
				metaPairMapBegin->second.printSummary();
			}
		}
		std::map<int, FusedPointMeta> metaPairMap;
};

#endif
