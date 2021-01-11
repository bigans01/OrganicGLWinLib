#pragma once

#ifndef COPLANARRELATIONSHIPTRACKER_H
#define COPLANARRELATIONSHIPTRACKER_H

#include <map>
#include "CoplanarRelationships.h"
#include "SPoly.h"
#include "PolyDebugLevel.h"
#include "PolyLogger.h"


class CoplanarRelationshipTracker
{

public:
	std::map<int, CoplanarRelationships> relationshipContainer;
	void insertCoplanarRelationship(int in_trackedSPolyID, SPoly* in_trackedSPolyRef, int in_relatedSPolyID, SPoly* in_relatedSPolyRef);
	void buildCoplanarCategorizedLines();
	void setDebugLevel(PolyDebugLevel in_polyDebugLevel);
private:
	PolyDebugLevel relationshipTrackerDebugLevel = PolyDebugLevel::NONE;
	PolyLogger relationshipTrackerLogger;
};

#endif
