#pragma once

#ifndef CUTTINGTRIANGLE_H
#define CUTTINGTRIANGLE_H

#include "STriangle.h"
#include "CuttingTriangleLine.h"
#include "OrganicGLWinUtils.h"
#include "QMVec3FindCentroidFacingNormal.h"

class CuttingTriangle
{
	public:
		CuttingTriangle() {};
		CuttingTriangle(STriangle in_sTriangle);
	private:
		friend class STriangleCutter;
		friend class CuttableTriangle;
		friend class CuttingTriangleManager;
		
		CuttingTriangleLine cuttingLines[3];
		void reset();
};

#endif
