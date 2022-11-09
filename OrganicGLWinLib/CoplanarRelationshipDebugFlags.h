#pragma once

#ifndef COPLANARRELATIONSHIPDEBUGFLAGS_H
#define COPLANARRELATIONSHIPDEBUGFLAGS_H

#include "DebugOption.h"
#include "PolyDebugLevel.h"
#include "DebugOptionSet.h"

class CoplanarRelationshipDebugFlags
{
	public:
		CoplanarRelationshipDebugFlags() {};
	private:
		friend class CoplanarRelationships;
		friend class CoplanarRelationshipTracker;
		friend class CoplanarFusionMachine;


		PolyDebugLevel dlPrintBorderLinesOfTrackedAndRelatedSPolys = PolyDebugLevel::NONE;
		std::map<int, DebugOptionSet> specificTrackedSTriangleDOSMap;
		std::map<int, DebugOptionSet> specificTrackedCutterCuttingTriangleDOSMap;

		void handleSpecificTrackedSPolyDebugOption(DebugOption in_debugOption);
		void handleSpecificTrackedSPolySTriangleDebugOption(int in_sTriangleID, DebugOption in_debugOption);
		void handleSpecificTrackedSPolyCutterCuttingTriangleDebugOption(int in_cuttingTriangleID, DebugOption in_debugOption);
};

#endif
