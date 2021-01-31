#pragma once

#ifndef MASSZONEPOINTCLIPPER_H
#define MASSZONEPOINTCLIPPER_H

#include <map>
#include "SPoly.h"
#include "SPolySupergroup.h"
#include "PolyLogger.h"
#include "PolyDebugLevel.h"
#include "MeshMatterMeta.h"
#include "PointToMassRelationshipMap.h"

class MassZonePointClipper
{


	private:	
		friend class MassZoneMaster;
		friend class MassZone;	
		std::map<int, SPoly*> clippingShellMap;
		std::map<int, MeshMatterMeta>* otherZoneMeshMatterMetaMapRef = nullptr;

		void insertSPolySuperGroupRefsIntoClippingShell(SPolySupergroup* in_sPolySuperGroupRef);
		void insertSPolyRefIntoClippingShell(SPoly* in_sPolyRef);
		void setOtherZoneMeshMatterMetaMapRef(std::map<int, MeshMatterMeta>* in_otherZoneMeshMatterMetaMapRef);
		void run();
		void compareMeshMatterMetaAgainstClippingShells(MeshMatterMeta* in_meshMatterMetaRef);
};

#endif
