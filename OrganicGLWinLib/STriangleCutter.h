#pragma once

#ifndef STRIANGLECUTTER_H
#define STRIANGLECUTTER_H

#include <map>
#include "STriangle.h"
#include "CuttableTriangleContainer.h"
#include "CuttingTriangleManager.h"
#include "STriangleOutputContainer.h"
#include "PolyLogger.h"
#include "PolyDebugLevel.h"
#include "DebugOptionSet.h"
#include "CuttingSequenceRunStatus.h"

class STriangleCutter
{
	public:
		void setCuttingParameters(STriangle in_sTriangle, CuttingTriangleManager* in_cuttingTriangleManagerRef);
		CuttingSequenceRunStatus runCuttingSequence();
		void setCuttableDOS(DebugOptionSet in_debugOptionSet);
		void setDebugOptionForSpecificCuttingTriangle(int in_cuttingTriangleID, DebugOption in_debugOption);
		void setCuttingTriangleDOSMap(std::map<int, DebugOptionSet> in_debugOptionSetMap);
	private:
		CuttableTriangleContainer cuttableContainer;
		CuttingTriangleManager* cuttingTrianglesRef = nullptr;
		std::map<int, DebugOptionSet> specificCuttingTriangleDOSMap;
		DebugOptionSet getDOSForSpecificCuttingTriangle(int in_cuttingTriangleID);
		DebugOptionSet cutterCuttableSTriangleDOS;	// debug options, for cuttable STriangles
		PolyDebugLevel checkForCutterDO(DebugOption in_debugOptionToFind);
};

#endif