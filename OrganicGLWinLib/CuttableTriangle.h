#pragma once

#ifndef CUTTABLETRIANGLE_H
#define CUTTABLETRIANGLE_H

#include "STriangle.h"
#include "CuttingTriangle.h"
#include "CuttableTriangleLine.h"
#include <map>
#include "OrganicGLWinUtils.h"
#include "QuatRotationPoints.h"
#include "QMVec3FindCentroidFacingNormal.h"
#include "TwoDLineSegment.h"
#include "TwoDLineSegmentIntersectAnalyzerV2.h"
#include "TwoDPoint.h"
#include <vector>
#include "TwoDCrawlingAttempt.h"
#include "CutLinePool.h"
#include "QMVec3FindCyclingDirectionPoint.h"
#include "CyclingDirection.h"
#include "PolyDebugLevel.h"
#include "PolyLogger.h"
#include "CutTriangleGroupBuilder.h"
#include "CutLineWelder.h"

class CuttableTriangle
{
	public:
		CuttableTriangle() {};
		CuttableTriangle(STriangle in_cuttableTriangle);
		void compareAgainstCuttingTriangle(CuttingTriangle* in_cuttingTriangleRef, int in_cuttingTriangleID, PolyDebugLevel in_polyDebugLevel);
	private:
		friend class STriangleOutputContainer;
		friend class STriangleCutter;
		std::map<int, STriangle> outputTriangles;
		CuttableTriangleLine cuttableTriangleLines[3];
		void compareCuttableTriangleLineToCuttingTriangleLine(int in_cuttableIndex, CuttableTriangleLine* in_cuttableTriangleLineRef, int in_cuttingIndex, CuttingTriangleLine* in_cuttingTriangleLineRef);
		glm::vec3 convert2DpointTo3D(TwoDPoint in_2dPoint);
		void printCuttableLineIntersections();
		void buildAllSlicingAttempts(CuttingTriangle* in_cuttingTriangleRef);
		void buildTypicalAttempts(CuttingTriangle* in_cuttingTriangleRef);
		void produceCutLinePoolsFromAttempts(CuttingTriangle* in_cuttingTriangleRef);
		CutLinePool buildLinesFromTypicalAttempt(TwoDCrawlingAttempt in_attempt, CuttingTriangle* in_cuttingTriangleRef);
		CutLinePool buildLinesFromSliceAttempt(TwoDCrawlingAttempt in_attempt, CuttingTriangle* in_cuttingTriangleRef);
		std::vector<TwoDCrawlingAttempt> crawlingAttemptsVector;
		bool testIfCuttingTriangleConsumesThisTriangle(CuttingTriangle* in_cuttingTriangleRef);
		bool testIfThisTriangleConsumesCuttingTriangle(CuttingTriangle* in_cuttingTriangleRef);
};

#endif
