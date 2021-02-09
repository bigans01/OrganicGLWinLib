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

class CuttableTriangle
{
	public:
		CuttableTriangle() {};
		CuttableTriangle(STriangle in_cuttableTriangle);
		void compareAgainstCuttingTriangle(CuttingTriangle* in_cuttingTriangleRef);
	private:
		friend class STriangleOutputContainer;
		friend class STriangleCutter;
		std::map<int, STriangle> outputTriangles;
		CuttableTriangleLine cuttableTriangleLines[3];
		void compareCuttableTriangleLineToCuttingTriangleLine(int in_cuttableIndex, CuttableTriangleLine* in_cuttableTriangleLineRef, int in_cuttingIndex, CuttingTriangleLine* in_cuttingTriangleLineRef);
		glm::vec3 convert2DpointTo3D(TwoDPoint in_2dPoint);
		void printCuttableLineIntersections();
		void checkForSlicingCondition(CuttingTriangle* in_cuttingTriangleRef);
		std::vector<TwoDCrawlingAttempt> crawlingAttemptsVector;
};

#endif
