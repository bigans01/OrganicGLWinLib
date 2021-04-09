#pragma once

#ifndef MASSZONEMASTER_H
#define MASSZONEMASTER_H

#include "MassZoneBoxType.h"
#include "MassZoneType.h"
#include "MassZone.h"
#include <map>
#include "SPoly.h"
#include "PolyLogger.h"
#include "PolyDebugLevel.h"
#include "DebugOption.h"
#include "MassManipulationMode.h"
#include "DebugOptionSet.h"

class MassZoneMaster
{
	public:
		void registerSPolyToMassZone(int in_sPolyID, SPoly in_sPoly, MassZoneType in_massZoneType);
		void disqualifyMeshMatterMeta(int in_sPolyID);
		void setDOForClippableSPolyInClipper(int in_sPolyID, DebugOption in_debugOption);
		void printQualifiedMeshMatterMetas();
		void createMassZoneBoxBoundaries(MassZoneBoxType in_massZoneBoxType);
		void setZoneClipperReferences();
		void setMassZoneLogLevels(PolyDebugLevel in_polyDebugLevel);
		void createMassZoneShells();
		void printMassZoneBorderLineCounts();
		void handleDebugOption(DebugOption in_debugOption);
		void runPointClippers();
		void setOldZoneClipperManipulationMode(MassManipulationMode in_massManipulationMode);
		void setNewZoneClipperManipulationMode(MassManipulationMode in_massManipulationMode);
	private:
		friend class SPolySet;
		std::map<int, MassZoneType> sPolyZoneTypeMap;
		std::map<int, DebugOptionSet> massZoneClippableOptionSets;
		MassZone oldZone;
		MassZone newZone;
		void sendClippableDOSetsToClippers();
};

#endif
