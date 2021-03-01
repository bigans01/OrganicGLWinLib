#pragma once

#ifndef COPLANARRELATIONSHIPTRACKER_H
#define COPLANARRELATIONSHIPTRACKER_H

#include <map>
#include "CoplanarRelationships.h"
#include "SPoly.h"
#include "PolyDebugLevel.h"
#include "PolyLogger.h"
#include "OperableIntSet.h"
#include "CoplanarRelationshipDebugFlags.h"


class CoplanarRelationshipTracker
{
	private:
		friend class SPolySet;

		PolyDebugLevel relationshipTrackerDebugLevel = PolyDebugLevel::NONE;
		PolyLogger relationshipTrackerLogger;
		OperableIntSet removableSPolys;
		std::map<int, CoplanarRelationshipDebugFlags> relationshipDebugFlags;
		std::map<int, CoplanarRelationships> relationshipContainer;

		void insertCoplanarRelationship(int in_trackedSPolyID, SPoly* in_trackedSPolyRef, int in_relatedSPolyID, SPoly* in_relatedSPolyRef);
		void runAllCuttingSequenceTests();
		void setDebugLevel(PolyDebugLevel in_polyDebugLevel);
		void insertDOForSpecificTrackedSPoly(int in_trackedSPolyID, DebugOption in_debugOption);
		void insertDOForSpecificTrackedSPolySTriangle(int in_trackedSPolyID, int in_trackedSPolySTriangleID, DebugOption in_debugOption);
		void insertDOForSpecificTrackedCutterCuttingTriangle(int in_trackedSPolyID, int in_trackedSPolyCuttingTriangleID, DebugOption in_debugOption);
		PolyDebugLevel retrieveDebugLevelOptionIfExistent(int in_trackedSPolyID);
		void insertRelationship(int in_trackedSPolyID, SPoly* in_trackedSPolyRef, int in_relatedSPolyID, SPoly* in_relatedSPolyRef);
		void applyCoplanarRelationshipDebugFlagsIfFound(int in_trackedSPolyID);
};

#endif
