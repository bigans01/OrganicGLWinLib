#pragma once

#ifndef CoplanarFusionMachine_H
#define CoplanarFusionMachine_H

#include "CoplanarRelationshipTracker.h"
#include "MassZoneType.h"
#include "MassZoneBoxType.h"
#include <map>
#include "CoplanarFusionType.h"
#include "PolyDebugLevel.h"
#include "OperableIntSet.h"
#include "CoplanarRelationshipDebugFlags.h"

class CoplanarFusionMachine
{
	private:
		friend class SPolySet;

		PolyDebugLevel coplanarFusionMachineLogLevel = PolyDebugLevel::NONE;
		PolyLogger coplanarFusionMachineLogger;
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

		MassZoneType fusionMachineMassZoneMode = MassZoneType::NOVAL;
		CoplanarRelationshipTracker oldSetTracker;
		CoplanarRelationshipTracker newSetTracker;
		CoplanarFusionType resultingFusionType = CoplanarFusionType::NOVAL;		// the fusion type that is determined after the call to determineFusionType.
		std::map<int, SPolyRefAndGroup> sPolyGroupMap;							// keeps track of which group an SPoly belongs to; also stores a direct reference to that SPoly in the SPolyset.
		OperableIntSet fusedRemovables;											// the SPolySet IDs of SPolys that the machine flags as removable.
		std::map<int, CoplanarRelationshipDebugFlags> fushionMachineRelationshipDebugFlags;	// set up via calls to various SPolySet setDO options

		void insertCoplanarRelationship(int in_polySetGroupID,					// if group ID = 0, insert into oldSetTracker. If 1, newSetTracker.
										int in_trackedSPolyID, 
										SPoly* in_trackedSPolyRef, 
										int in_relatedSPolyID, 
										SPoly* in_relatedSPolyRef);
		void printRelationshipsInTrackers();
		void insertSPolyGroupMapLink(int in_sPolyID, int in_groupID, SPoly* in_sPolyRef);
		void determineFusionType();
		void runAllCuttingSequenceTests();
		void setCoplanarFusionMachineDebugLevel(PolyDebugLevel in_debugLevel);
		void setMassZoneMode(MassZoneBoxType in_massZoneBoxType);

		// debug setting functions
		void fmInsertDOForSpecificTrackedSPoly(int in_trackedSPolyID, DebugOption in_debugOption);
		void fmInsertDOForSpecificTrackedSPolySTriangle(int in_trackedSPolyID, int in_trackedSPolySTriangleID, DebugOption in_debugOption);
		void fmInsertDOForSpecificTrackedCutterCuttingTriangle(int in_trackedSPolyID, int in_trackedSPolyCuttingTriangleID, DebugOption in_debugOption);

};

#endif
