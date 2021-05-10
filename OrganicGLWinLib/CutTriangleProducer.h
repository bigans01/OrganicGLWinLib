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

			// the below funtion was commented out on 5/8/2021; it needs to be reviwed/reworked.
			//checkIfAnyComparablesPointBAreInCandidateTriangle();	
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

			//std::cout << "(CutTriangleProducer) ::::::: prior to check if point is within Triangle: " << std::endl;

			//std::cout << "::::::: prior to check if point is within Triangle: " << std::endl;
			//std::cout << "(CutTriangleProducer) ::::::: prior to check if point is within Triangle: " << std::endl;
			//int someVal = 3;
			//candidateCutTriangle.printCutTrianglePoints();
		}

		void checkIfFrontHookPointBIsInCandidateTriangle()
		{
			glm::vec3 pointToCheck = cutLinePoolRef->fetchLineFromPoolViaIndex(linePoolGuide.frontHook).pointB;
			//std::cout << "(CutTriangleProducer) ::::::: point to check will be: " << pointToCheck.x << ", " << pointToCheck.y << ", " << pointToCheck.z << std::endl;
			bool wasWithinTriangle = candidateCutTriangle.checkIfPointIsWithinTriangle(pointToCheck);
			if (wasWithinTriangle == true)
			{
				//std::cout << "::::::::: detected as being within triangle. flagging to false." << std::endl;
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
					std::cout << "Line " <<  *comparablesBegin << "found as being within!! " << std::endl;
					std::cout << "Point was: " << pointToCheck.x << ", " << pointToCheck.y << ", " << pointToCheck.z << std::endl;
					wasValidCutTriangleProduced = false;
				}
			}
		}
};


#endif
