#pragma once

#ifndef COPLANARRELATIONSHIPTRACKER_H
#define COPLANARRELATIONSHIPTRACKER_H

#include <map>
#include "CoplanarRelationships.h"
#include "SPoly.h"


class CoplanarRelationshipTracker
{

public:
	std::map<int, CoplanarRelationships> relationshipContainer;
	void insertCoplanarRelationship(int in_trackedSPolyID, SPoly* in_trackedSPolyRef, int in_relatedSPolyID, SPoly* in_relatedSPolyRef);
	void buildCoplanarCategorizedLines();
	void buildCategorizedLinesForCoplanarRelationship(int in_trackedSPolyID);
};

#endif
