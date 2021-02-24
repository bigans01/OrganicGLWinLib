#pragma once

#ifndef CUTTABLETRIANGLE_H
#define CUTTABLETRIANGLE_H

#include "STriangle.h"
#include "CuttingTriangle.h"
#include "CuttableTriangleLine.h"
#include <map>
#include "OrganicGLWinUtils.h"
#include "QuatRotationPoints.h"
#include "TwoDLineSegment.h"
#include "TwoDLineSegmentIntersectAnalyzerV2.h"
#include "TwoDPoint.h"
#include <vector>
#include "TwoDCrawlingAttempt.h"
#include "CutLinePool.h"
#include "CyclingDirection.h"
#include "PolyDebugLevel.h"
#include "PolyLogger.h"
#include "CutTriangleGroupBuilder.h"
#include "CutLineWelder.h"
#include "QuatUtils.h"

class CuttableTriangle
{
	public:
		CuttableTriangle() {};
	private:
		//CuttableTriangle() {};
		CuttableTriangle(STriangle in_cuttableTriangle);
		void compareAgainstCuttingTriangle(CuttingTriangle* in_cuttingTriangleRef, int in_cuttingTriangleID, PolyDebugLevel in_polyDebugLevel);
		friend class STriangleOutputContainer;
		friend class STriangleCutter;
		friend class CutLineWelder;
		friend class CuttableTriangleContainer;

		struct PoolAndDirectionPair
		{
			CutLinePool pairPool;
			CyclingDirection pairCyclingDirection = CyclingDirection::NOVAL;
			//bool isAttemptValid = true;
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

		struct PotentialLineColinearityResult
		{
			bool isColinear = false;
		};

		glm::vec3 fetchThirdPoint(CuttablePointPair in_cuttablePointPair);
		PotentialLineColinearityResult acquireColinearityResult(glm::vec3 in_thirdPoint, glm::vec3 in_intersectingPoint, glm::vec3 in_cuttingLinePointA, glm::vec3 in_cuttingLinePointB);

		std::map<int, STriangle> outputTriangles;
		CuttableTriangleLine cuttableTriangleLines[3];
		void compareCuttableTriangleLineToCuttingTriangleLine(int in_cuttableIndex, CuttableTriangleLine* in_cuttableTriangleLineRef, int in_cuttingIndex, CuttingTriangleLine* in_cuttingTriangleLineRef);
		glm::vec3 convert2DpointTo3D(TwoDPoint in_2dPoint);
		void printCuttableLineIntersections();
		void buildAllSlicingAttempts(CuttingTriangle* in_cuttingTriangleRef);
		void buildTypicalAttempts(CuttingTriangle* in_cuttingTriangleRef);
		void produceCutLinePoolsFromAttempts(CuttingTriangle* in_cuttingTriangleRef);
		PoolAndDirectionPair buildLinesFromTypicalAttempt(TwoDCrawlingAttempt in_attempt, CuttingTriangle* in_cuttingTriangleRef);
		PoolAndDirectionPair buildLinesFromSliceAttempt(TwoDCrawlingAttempt* in_attempt, CuttingTriangle* in_cuttingTriangleRef);
		std::vector<TwoDCrawlingAttempt> crawlingAttemptsVector;
		bool testIfCuttingTriangleConsumesThisTriangle(CuttingTriangle* in_cuttingTriangleRef);
		bool testIfThisTriangleConsumesCuttingTriangle(CuttingTriangle* in_cuttingTriangleRef);
		void convertAndStoreCutTriangleVector(std::vector<CutTriangle> in_vector);
		STriangle convertCutTriangleToSTriangle(CutTriangle in_cutTriangle);
		void printCuttableTrianglePoints();
};

#endif
