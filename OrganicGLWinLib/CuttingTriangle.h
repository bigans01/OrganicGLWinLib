#pragma once

#ifndef CUTTINGTRIANGLE_H
#define CUTTINGTRIANGLE_H

#include "STriangle.h"
#include "CuttingTriangleLine.h"
#include "OrganicGLWinUtils.h"
#include "QuatUtils.h"

class CuttingTriangle
{
	public:
		CuttingTriangle() {};
		CuttingTriangle(STriangle in_sTriangle);
	private:
		friend class STriangleCutter;
		friend class CuttableTriangle;
		friend class CuttingTriangleManager;
		friend class CutLineWelder;
		
		CuttingTriangleLine cuttingLines[3];
		void reset();
		void printCuttingLineIntersections();
		void printPoints();
};

#endif
