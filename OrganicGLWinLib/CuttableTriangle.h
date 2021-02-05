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

class CuttableTriangle
{
	public:
		void buildCuttableTriangle(STriangle in_cuttableTriangle);
		void compareAgainstCuttingTriangle(CuttingTriangle* in_cuttingTriangleRef);
	private:
		friend class STriangleOutputContainer;
		friend class STriangleCutter;
		std::map<int, STriangle> outputTriangles;
		CuttableTriangleLine cuttableTriangleLines[3];
		void compareCuttableTriangleLineToCuttingTriangleLine(int in_cuttableIndex, CuttableTriangleLine* in_cuttableTriangleLineRef, int in_cuttingIndex, CuttingTriangleLine* in_cuttingTriangleLineRef);
};

#endif
