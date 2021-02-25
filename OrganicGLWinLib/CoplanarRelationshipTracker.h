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
		std::map<int, CoplanarRelationships> relationshipContainer;
		void insertCoplanarRelationship(int in_trackedSPolyID, SPoly* in_trackedSPolyRef, int in_relatedSPolyID, SPoly* in_relatedSPolyRef);
		void runAllCuttingSequenceTests();
		void setDebugLevel(PolyDebugLevel in_polyDebugLevel);
		friend class SPolySet;
		PolyDebugLevel relationshipTrackerDebugLevel = PolyDebugLevel::NONE;
		PolyLogger relationshipTrackerLogger;
		OperableIntSet removableSPolys;
};

#endif
