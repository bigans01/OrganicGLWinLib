#pragma once

#ifndef MASSZONEPOINTCLIPPER_H
#define MASSZONEPOINTCLIPPER_H

#include <map>
#include <set>
#include "SPoly.h"
#include "SPolySupergroup.h"
#include "PolyLogger.h"
#include "PolyDebugLevel.h"
#include "MeshMatterMeta.h"
#include "PointToMassRelationshipMap.h"
#include "PointToSPolyRelationshipTrackerContainer.h"
#include "QuatRotationPoints.h"
#include "QMBoolPointWithinTrianglePBZ.h"
#include "OperableIntSet.h"
#include "CoplanarChecker.h"
#include "BorderLineLinkContainer.h"
#include "FusionCandidateProducer.h"
#include "FusionCandidate.h"
#include "PolyLogger.h"
#include "PolyDebugLevel.h"
#include "QuatUtils.h"

class MassZonePointClipper
{
	private:	
		friend class MassZoneMaster;
		friend class MassZone;
		friend class SPolySet;
		std::map<int, SPoly*> clippingShellMap;
		std::map<int, MeshMatterMeta>* otherZoneMeshMatterMetaMapRef = nullptr;
		std::set<int> sPolysToPurge;		// stores the SPolySet ID(s) of any SPoly that needs to be purged.
		void insertSPolySuperGroupRefsIntoClippingShell(SPolySupergroup* in_sPolySuperGroupRef);
		void insertSPolyRefIntoClippingShell(SPoly* in_sPolyRef);
		void setOtherZoneMeshMatterMetaMapRef(std::map<int, MeshMatterMeta>* in_otherZoneMeshMatterMetaMapRef);
		void run();
		bool compareMeshMatterMetaAgainstClippingShells(MeshMatterMeta* in_meshMatterMetaRef);
		bool checkIfPointIsWithinPBZ(glm::vec3 in_pointToCheck, STriangle in_sTriangleCopy);
		bool runFirstTwoDisqualificationPasses(BorderLineLinkContainer* in_borderLineLinkContainerRef, PointToSPolyRelationshipTrackerContainer* in_trackerContainerRef);
		void setClipperDebugLevel(PolyDebugLevel in_polyDebugLevel, std::string in_zoneString);
		PolyLogger clipperPolyLogger;
		PolyDebugLevel clipperPolyLoggerDebugLevel = PolyDebugLevel::NONE;
		std::string zoneString = "";		// would be set, when calling setDebugLevel.
};

#endif
