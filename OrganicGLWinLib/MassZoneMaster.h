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

class MassZoneMaster
{
	public:
		void registerSPolyToMassZone(int in_sPolyID, SPoly in_sPoly, MassZoneType in_massZoneType);
		void disqualifyMeshMatterMeta(int in_sPolyID);
		void printQualifiedMeshMatterMetas();
		void createMassZoneBoxBoundaries(MassZoneBoxType in_massZoneBoxType);
		void setZoneClipperReferences();
		void setMassZoneLogLevels(PolyDebugLevel in_polyDebugLevel);
		void createMassZoneShells();
		void printMassZoneBorderLineCounts();
		void handleDebugOption(DebugOption in_debugOption);
		void runPointClippers();
	private:
		std::map<int, MassZoneType> sPolyZoneTypeMap;
		MassZone oldZone;
		MassZone newZone;
};

#endif
