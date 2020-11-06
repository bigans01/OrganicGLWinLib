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
		}
	private:
		WeldedLinePool* weldedLinePoolRef = nullptr;
		WeldedLinePoolGuide linePoolGuide;
		WeldedLine newOrientedLine;
		WeldedTriangle candidateWeldedTriangle;

		bool wasValidWeldedTriangleProduced = true;		
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
		}
};

#endif
