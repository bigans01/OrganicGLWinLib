#pragma once

#ifndef COPLANARRELATIONSHIPTRACKER_H
#define COPLANARRELATIONSHIPTRACKER_H

#include <map>
#include "CoplanarRelationships.h"
#include "SPoly.h"
#include "PolyDebugLevel.h"
#include "PolyLogger.h"
#include "OperableIntSet.h"


class CoplanarRelationshipTracker
{
	private:
		friend class SPolySet;
		std::map<int, CoplanarRelationships> relationshipContainer;
		void insertCoplanarRelationship(int in_trackedSPolyID, SPoly* in_trackedSPolyRef, int in_relatedSPolyID, SPoly* in_relatedSPolyRef);
		void runAllCuttingSequenceTests();
		void setDebugLevel(PolyDebugLevel in_polyDebugLevel);
		void insertDebugLevelOptionForSpecificTrackedSPoly(int in_trackedSPolyID, PolyDebugLevel in_polyDebugLevel);
		PolyDebugLevel retrieveDebugLevelOptionIfExistent(int in_trackedSPolyID);
		void insertRelationship(int in_trackedSPolyID, SPoly* in_trackedSPolyRef, int in_relatedSPolyID, SPoly* in_relatedSPolyRef);
		PolyDebugLevel relationshipTrackerDebugLevel = PolyDebugLevel::NONE;
		PolyLogger relationshipTrackerLogger;
		OperableIntSet removableSPolys;
		std::map<int, PolyDebugLevel> specificRelationshipOptions;
};

#endif
