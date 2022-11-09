#pragma once

#ifndef CUTTABLETRIANGLE_H
#define CUTTABLETRIANGLE_H

#include "STriangle.h"
#include "CuttingTriangle.h"
#include "CuttableTriangleLine.h"
#include "OrganicGLWinUtils.h"
#include "QuatRotationPoints.h"
#include "TwoDLineSegment.h"
#include "TwoDLineSegmentIntersectAnalyzerV2.h"
#include "TwoDPoint.h"
#include "TwoDCrawlingAttempt.h"
#include "CutLinePool.h"
#include "CyclingDirection.h"
#include "PolyDebugLevel.h"
#include "PolyLogger.h"
#include "CutTriangleGroupBuilder.h"
#include "CutLineWelder.h"
#include "QuatUtils.h"
#include "DebugOptionSet.h"
#include "ErrorSensor.h"

class CuttableTriangle
{
	public:
		CuttableTriangle() {};
	private:
		CuttableTriangle(STriangle in_cuttableTriangle);
		friend class STriangleOutputContainer;
		friend class STriangleCutter;
		friend class CutLineWelder;
		friend class CuttableTriangleContainer;

		struct PoolAndDirectionPair
		{
			CutLinePool pairPool;
			CyclingDirection pairCyclingDirection = CyclingDirection::NOVAL;
			bool isAttemptValid = false;
		};

		struct CuttablePointPair
		{
			CuttablePointPair(glm::vec3 in_point0, glm::vec3 in_point1)
			{
				pointArray[0] = in_point0;
				pointArray[1] = in_point1;
			};
			CuttablePointPair() {};
			glm::vec3 pointArray[2];
		};

		glm::vec3 fetchThirdPoint(CuttablePointPair in_cuttablePointPair);
		std::vector<TwoDCrawlingAttempt> crawlingAttemptsVector;
		std::map<int, STriangle> outputTriangles;
		CuttableTriangleLine cuttableTriangleLines[3];
		DebugOptionSet cuttingTriangleDOS;

		ErrorSensor compareAgainstCuttingTriangle(CuttingTriangle* in_cuttingTriangleRef, int in_cuttingTriangleID, DebugOptionSet in_cuttingTriangledebugOptionSet);
		void compareCuttableTriangleLineToCuttingTriangleLine(int in_cuttableIndex, CuttableTriangleLine* in_cuttableTriangleLineRef, int in_cuttingIndex, CuttingTriangleLine* in_cuttingTriangleLineRef);
		glm::vec3 convert2DpointTo3D(TwoDPoint in_2dPoint);
		void printCuttableLineIntersections();
		void buildAllSlicingAttempts(CuttingTriangle* in_cuttingTriangleRef);
		void buildTypicalAttempts(CuttingTriangle* in_cuttingTriangleRef);
		ErrorSensor produceCutTriangles(CuttingTriangle* in_cuttingTriangleRef);
		PoolAndDirectionPair buildLinesFromTypicalAttempt(TwoDCrawlingAttempt in_attempt, CuttingTriangle* in_cuttingTriangleRef);
		PoolAndDirectionPair buildLinesFromSliceAttempt(TwoDCrawlingAttempt* in_attempt, CuttingTriangle* in_cuttingTriangleRef);
		bool testIfCuttingTriangleConsumesThisTriangle(CuttingTriangle* in_cuttingTriangleRef);
		bool testIfThisTriangleConsumesCuttingTriangle(CuttingTriangle* in_cuttingTriangleRef);
		void convertAndStoreCutTriangleVector(std::vector<CutTriangle> in_vector);
		STriangle convertCutTriangleToSTriangle(CutTriangle in_cutTriangle);
		void printCuttableTrianglePoints();
		PolyDebugLevel checkForCuttingTriangleDO(DebugOption in_debugOptionToFind);	// returns PolyDebugLevel::DEBUG if a the passed-in DebugOption is found.
};

#endif
