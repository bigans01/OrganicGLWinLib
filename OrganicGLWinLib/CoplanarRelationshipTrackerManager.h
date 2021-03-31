#pragma once

#ifndef COPLANARRELATIONSHIPTRACKERMANAGER_H
#define COPLANARRELATIONSHIPTRACKERMANAGER_H

#include "CoplanarRelationshipTracker.h"

class CoplanarRelationshipTrackerManager
{
	private:
		friend class SPolySet;

		//PolyDebugLevel relationshipTrackerDebugLevel = PolyDebugLevel::NONE;
		//PolyLogger relationshipTrackerLogger;

		CoplanarRelationshipTracker oldSetTracker;
		CoplanarRelationshipTracker newSetTracker;

		void insertCoplanarRelationship(int in_polySetGroupID, int in_trackedSPolyID, SPoly* in_trackedSPolyRef, int in_relatedSPolyID, SPoly* in_relatedSPolyRef);
};

#endif
