#pragma once

#ifndef WELDEDTRIANGLEPRODUCER_H
#define WELDEDTRIANGLEPRODUCER_H

#include "WeldedLinePool.h"
#include "WeldedLinePoolGuide.h"
#include "WeldedLine.h"
#include "WeldedTriangle.h"

class WeldedTriangleProducer
{
	public:
		WeldedTriangleProducer(WeldedLinePool* in_weldedLinePoolRef, WeldedLinePoolGuide in_weldedLinePoolGuide) :
			weldedLinePoolRef(in_weldedLinePoolRef),
			linePoolGuide(in_weldedLinePoolGuide)
		{
			buildCandidateTriangle();
			checkIfFrontHookPointBIsInCandidateTriangle();
			checkIfAnyComparablesPointBAreInCandidateTriangle();
		}
		bool wasValidWeldedTriangleProduced = true;
	private:
		WeldedLinePool* weldedLinePoolRef = nullptr;
		WeldedLinePoolGuide linePoolGuide;
		WeldedLine newOrientedLine;
		WeldedTriangle candidateWeldedTriangle;

		
		void buildCandidateTriangle()
		{
			WeldedLine candidateLineOfSight = weldedLinePoolRef->fetchLineFromPoolViaIndex(linePoolGuide.lineOfSightLineIndex);
			WeldedLine candidateRearHook = weldedLinePoolRef->fetchLineFromPoolViaIndex(linePoolGuide.rearHook);
			WeldedTriangle candidateTriangle(candidateLineOfSight, candidateRearHook);
			candidateWeldedTriangle = candidateTriangle;
		}

		void checkIfFrontHookPointBIsInCandidateTriangle()
		{
			glm::vec3 pointToCheck = weldedLinePoolRef->fetchLineFromPoolViaIndex(linePoolGuide.frontHook).pointB;
			bool wasWithinTriangle = candidateWeldedTriangle.checkIfPointIsWithinTriangle(pointToCheck);
			if (wasWithinTriangle == true)
			{
				wasValidWeldedTriangleProduced = false;
			}
		}

		void checkIfAnyComparablesPointBAreInCandidateTriangle()
		{
			auto comparablesBegin = linePoolGuide.comparables.begin();
			auto comparablesEnd = linePoolGuide.comparables.end();
			for (; comparablesBegin != comparablesEnd; comparablesBegin++)
			{
				std::cout << "Checking against comparable line, having the ID: " << *comparablesBegin << std::endl;

				glm::vec3 pointToCheck = weldedLinePoolRef->fetchLineFromPoolViaIndex(*comparablesBegin).pointB;
				bool wasWithinTriangle = candidateWeldedTriangle.checkIfPointIsWithinTriangle(pointToCheck);
				if (wasWithinTriangle == true)
				{
					wasValidWeldedTriangleProduced = false;
				}
			}
		}
};

#endif
