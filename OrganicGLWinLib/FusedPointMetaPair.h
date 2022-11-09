#pragma once

#ifndef FUSEDPOINTMETAPAIR_H
#define FUSEDPOINTMETAPAIR_H

#include "FusedPointMeta.h"
#include "SummaryScanResult.h"
#include "FusionCandidateOrigin.h"

class FusedPointMetaPair
{
	public:
		void insertFusedPointMeta(FusedPointMeta in_fusedPointMeta)
		{
			int currentIndex = int(metaPairMap.size());
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
		SummaryScanResult searchForSummary(FusedPointSummary in_fusedPointSummary)
		{
			SummaryScanResult returnResult;
			auto metaPairMapBegin = metaPairMap.begin();
			auto metaPairMapEnd = metaPairMap.end();
			for (; metaPairMapBegin != metaPairMapEnd; metaPairMapBegin++)
			{
				if (metaPairMapBegin->second.summary == in_fusedPointSummary)	// it was found
				{
					returnResult.wasFound = true;
					returnResult.foundIndex = metaPairMapBegin->first;
					returnResult.foundPoint = metaPairMapBegin->second.point;
					returnResult.pointSummary = in_fusedPointSummary;
				}
			}
			return returnResult;
		}

		SummaryScanResult searchForSummaryInSpecifiedOrigin(FusedPointSummary in_fusedPointSummary, FusionCandidateOrigin in_originToMatch)
		{
			SummaryScanResult returnResult;
			auto metaPairMapBegin = metaPairMap.begin();
			auto metaPairMapEnd = metaPairMap.end();
			for (; metaPairMapBegin != metaPairMapEnd; metaPairMapBegin++)
			{
				if 
				(
					(metaPairMapBegin->second.summary == in_fusedPointSummary)	// it was found
					&&
					(metaPairMapBegin->second.origin == in_originToMatch)
				)
				{
					returnResult.wasFound = true;
					returnResult.foundIndex = metaPairMapBegin->first;
					returnResult.foundPoint = metaPairMapBegin->second.point;
					returnResult.pointSummary = in_fusedPointSummary;
				}
			}
			
			if (returnResult.wasFound == false)
			{
				//std::cout << "(SummaryScanResult): notice: originToMatch didn't match anything..." << std::endl;
			}
			return returnResult;
		}

		SummaryScanResult searchForOtherSummary(glm::vec3 in_fusedPointNotToMatch)
		{
			SummaryScanResult returnResult;
			auto metaPairMapBegin = metaPairMap.begin();
			auto metaPairMapEnd = metaPairMap.end();
			for (; metaPairMapBegin != metaPairMapEnd; metaPairMapBegin++)
			{
				if (metaPairMapBegin->second.point != in_fusedPointNotToMatch)	// it was found
				{
					returnResult.wasFound = true;
					returnResult.foundIndex = metaPairMapBegin->first;
					returnResult.foundPoint = metaPairMapBegin->second.point;
					returnResult.pointSummary = metaPairMapBegin->second.summary;
				}
			}
			return returnResult;
		}

};

#endif
