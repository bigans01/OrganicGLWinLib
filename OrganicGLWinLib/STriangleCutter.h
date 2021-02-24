#pragma once

#ifndef STRIANGLECUTTER_H
#define STRIANGLECUTTER_H

#include <map>
#include "STriangle.h"
#include "CuttableTriangleContainer.h"
#include "CuttingTriangleManager.h"
#include "STriangleOutputContainer.h"
#include "PolyDebugLevel.h"

class STriangleCutter
{
	public:
		void setCuttingParameters(STriangle in_sTriangle, CuttingTriangleManager* in_cuttingTriangleManagerRef);
		void setDebugLevelForSpecificCuttingTriangle(int in_cuttingTriangleID, PolyDebugLevel in_polyDebugLevel);
		bool runCuttingSequence();
	private:
		CuttableTriangleContainer cuttableContainer;
		CuttingTriangleManager* cuttingTrianglesRef = nullptr;
		std::map<int, PolyDebugLevel> specificCuttingTriangleOptionMap;
		PolyDebugLevel getPolyDebugLevelForSpecificCuttingTriangle(int in_cuttingTriangleID);
};

#endif