#pragma once

#ifndef CoplanarFusionMachine_H
#define CoplanarFusionMachine_H

#include "CoplanarRelationshipTracker.h"
#include "MassZoneType.h"
#include <map>

class CoplanarFusionMachine
{
	private:
		friend class SPolySet;

		//PolyDebugLevel relationshipTrackerDebugLevel = PolyDebugLevel::NONE;
		//PolyLogger relationshipTrackerLogger;
		struct SPolyRefAndGroup
		{
			SPolyRefAndGroup() {};
			SPolyRefAndGroup(int in_groupID, SPoly* in_sPolyRef) :
				groupID(in_groupID),
				sPolyRef(in_sPolyRef)
			{};

			public:
				int groupID = 0;
				SPoly* sPolyRef = nullptr;
		};

		CoplanarRelationshipTracker oldSetTracker;
		CoplanarRelationshipTracker newSetTracker;
		std::map<int, SPolyRefAndGroup> sPolyGroupMap;					// keeps track of which group an SPoly belongs to; also stores a direct reference to that SPoly in the SPolyset.

		void insertCoplanarRelationship(int in_polySetGroupID,			// if group ID = 0, insert into oldSetTracker. If 1, newSetTracker.
										int in_trackedSPolyID, 
										SPoly* in_trackedSPolyRef, 
										int in_relatedSPolyID, 
										SPoly* in_relatedSPolyRef);
		void printRelationshipsInTrackers();
		void insertSPolyGroupMapLink(int in_sPolyID, int in_groupID, SPoly* in_sPolyRef);
		void determineFusionType();
};

#endif
