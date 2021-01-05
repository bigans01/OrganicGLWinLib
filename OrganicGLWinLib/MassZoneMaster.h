#pragma once

#ifndef MASSZONEMASTER_H
#define MASSZONEMASTER_H

#include "MassZoneType.h"
#include "MassZone.h"
#include <map>
#include "SPoly.h"

class MassZoneMaster
{
	public:
		void registerSPolyToMassZone(int in_sPolyID, SPoly in_sPoly, MassZoneType in_massZoneType);
		void disqualifyMeshMatterMeta(int in_sPolyID);
		void printQualifiedMeshMatterMetas();
	private:
		std::map<int, MassZoneType> sPolyZoneTypeMap;
		MassZone oldZone;
		MassZone newZone;
};

#endif
