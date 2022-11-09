#pragma once

#ifndef MASSZONEPOINTCLIPPER_H
#define MASSZONEPOINTCLIPPER_H

#include "SPoly.h"
#include "SPolySupergroup.h"
#include "PolyLogger.h"
#include "PolyDebugLevel.h"
#include "MeshMatterMeta.h"
#include "PointToMassRelationshipMap.h"
#include "PointToSPolyRelationshipTrackerContainer.h"
#include "QuatRotationPoints.h"
#include "QMBoolPointWithinTrianglePBZ.h"
#include "CoplanarChecker.h"
#include "BorderLineLinkContainer.h"
#include "FusionCandidateProducer.h"
#include "FusionCandidate.h"
#include "PolyLogger.h"
#include "PolyDebugLevel.h"
#include "QuatUtils.h"
#include "MassManipulationMode.h"
#include "DebugOption.h"
#include "DebugOptionSet.h"

class MassZonePointClipper
{
	private:	
		friend class MassZoneMaster;
		friend class MassZone;
		friend class SPolySet;
		std::map<int, DebugOptionSet> specificClippableSPolyOptions;	// settings for the debug options on a specific SPoly that is clippable for this MassZone's shell.
		std::map<int, SPoly*> clippingShellMap;
		std::map<int, MeshMatterMeta>* otherZoneMeshMatterMetaMapRef = nullptr;
		std::set<int> sPolysToPurge;		// stores the SPolySet ID(s) of any SPoly that needs to be purged.
		OperableIntSet fissionTargets;		// any "nacho dip" SPolys found within this mass must be kept track of.
		MassManipulationMode clipperManipulationMode = MassManipulationMode::NOVAL;
		void insertSPolySuperGroupRefsIntoClippingShell(SPolySupergroup* in_sPolySuperGroupRef);
		void insertSPolyRefIntoClippingShell(SPoly* in_sPolyRef);
		void setOtherZoneMeshMatterMetaMapRef(std::map<int, MeshMatterMeta>* in_otherZoneMeshMatterMetaMapRef);
		void setClippableSPolyDO(int in_sPoly, DebugOption in_debugOption);		// sets options for a clippable SPoly in this clipper, if that SPoly is found.
		void run();
		bool compareMeshMatterMetaAgainstClippingShells(MeshMatterMeta* in_meshMatterMetaRef);
		bool checkIfPointIsWithinPBZ(glm::vec3 in_pointToCheck, STriangle in_sTriangleCopy);
		bool runFirstTwoDisqualificationPasses(BorderLineLinkContainer* in_borderLineLinkContainerRef, PointToSPolyRelationshipTrackerContainer* in_trackerContainerRef);
		void setClipperDebugLevel(PolyDebugLevel in_polyDebugLevel, std::string in_zoneString);
		void printClippingShellMapPoints();
		void printClippingShellMapPointsViaSet(OperableIntSet in_sPolyIDs);
		PolyLogger clipperPolyLogger;
		PolyDebugLevel clipperPolyLoggerDebugLevel = PolyDebugLevel::NONE;
		PolyDebugLevel checkForSpecificDOInSPoly(int in_sPoly, DebugOption in_debugOptionToCheck);
		std::string zoneString = "";		// would be set, when calling setDebugLevel.
};

#endif
