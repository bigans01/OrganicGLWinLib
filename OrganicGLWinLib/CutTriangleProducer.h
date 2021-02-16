#pragma once

#ifndef CUTTRIANGLEPRODUCER_H
#define CUTTRIANGLEPRODUCER_H

#include "CutLinePool.h"
#include "CutLinePoolGuide.h"
#include "CutLine.h"
#include "CutTriangle.h"
#include "CutTriangleProductionResult.h"

class CutTriangleProducer
{
	public:
		CutTriangleProducer(CutLinePool* in_cutLinePoolRef, CutLinePoolGuide in_cutLinePoolGuide) :
			cutLinePoolRef(in_cutLinePoolRef),
			linePoolGuide(in_cutLinePoolGuide)
		{
			buildCandidateTriangle();
			checkIfFrontHookPointBIsInCandidateTriangle();
			checkIfAnyComparablesPointBAreInCandidateTriangle();
		};
		bool wasValidCutTriangleProduced = true;
		CutTriangleProductionResult getProductionResult()
		{
			CutTriangleProductionResult result(wasValidCutTriangleProduced, candidateCutTriangle);
			return result;
		}

	private:
		CutLinePool* cutLinePoolRef = nullptr;
		CutLinePoolGuide linePoolGuide;
		CutLine newOrientedLine;
		CutTriangle candidateCutTriangle;

		void buildCandidateTriangle()
		{
			CutLine candidateLineOfSight = cutLinePoolRef->fetchLineFromPoolViaIndex(linePoolGuide.lineOfSightLineIndex);
			CutLine candidateRearHook = cutLinePoolRef->fetchLineFromPoolViaIndex(linePoolGuide.rearHook);
			CutTriangle candidateTriangle(candidateLineOfSight, candidateRearHook);
			candidateCutTriangle = candidateTriangle;
		}

		void checkIfFrontHookPointBIsInCandidateTriangle()
		{
			glm::vec3 pointToCheck = cutLinePoolRef->fetchLineFromPoolViaIndex(linePoolGuide.frontHook).pointB;
			bool wasWithinTriangle = candidateCutTriangle.checkIfPointIsWithinTriangle(pointToCheck);
			if (wasWithinTriangle == true)
			{
				wasValidCutTriangleProduced = false;
			}
		}

		void checkIfAnyComparablesPointBAreInCandidateTriangle()
		{
			auto comparablesBegin = linePoolGuide.comparables.begin();
			auto comparablesEnd = linePoolGuide.comparables.end();
			for (; comparablesBegin != comparablesEnd; comparablesBegin++)
			{
				//std::cout << "Checking against comparable line, having the ID: " << *comparablesBegin << std::endl;

				glm::vec3 pointToCheck = cutLinePoolRef->fetchLineFromPoolViaIndex(*comparablesBegin).pointB;
				bool wasWithinTriangle = candidateCutTriangle.checkIfPointIsWithinTriangle(pointToCheck);
				if (wasWithinTriangle == true)
				{
					wasValidCutTriangleProduced = false;
				}
			}
		}
};


#endif
