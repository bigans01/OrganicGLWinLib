#pragma once

#ifndef MASSZONE_H
#define MASSZONE_H

#include "MassZoneBox.h"
#include "MassSubZone.h"
#include "MeshMatterMeta.h"
#include "MassZoneBoxType.h"
#include <map>
#include <glm/glm.hpp>
#include "PolyDebugLevel.h"
#include "SPolyDOSet.h"
#include "BoundaryOrientation.h"
#include "MassZoneType.h"
#include "MassZonePointClipper.h"
#include "DebugOption.h"
#include "MessageContainer.h"
#include "MassUtils.h"
#include "ExceptionRecorder.h"


class MassZone
{
private:
	friend class MassZoneMaster;
	friend class SPolySet;
	friend class SPolyShellProducer;
	MassZoneBox zoneBox;
	MassZoneBoxType boxType = MassZoneBoxType::NOVAL;	// the "format" or size of the zone; BLOCK, ENCLAVE, COLLECTION;
														// must be set during the call to createMassZoneBoxBoundary; this value is needed
														// when calling the class that will attempt to resolve bad CleaveSequence generation
														// (this class hasn't been given a name, as of 1/29/2022)
	ExceptionRecorder* massZoneRecorderRef = nullptr;

	std::map<int, MassSubZone> subZoneMap;
	std::map<BoundaryOrientation, SPolyDOSet> boundaryDebugOptions;
	std::set<BoundaryOrientation> boxFaceProductionPermits;
	std::set<BoundaryOrientation> massZoneExcludedBoundaries;	// a set of BoundaryOrientations that represents
																// the correlating MassZoneBoxBoundary instances that exist in MassZoneBox.
																// If a BoundaryOrientation is in this set, building SPolys on that boundary will be ignored.
	std::map<BoundaryOrientation, SPolySupergroup>* excludedLegitBoundarySupergroupsRef = nullptr;	// a map of supergroups, where each supergroup contains normally-generated (non SCAB) boundary SPolys.
																									// This is needed by step 3-B of the MassZone::createMassZoneShell function,
																									// in order to manually insert the already generated, valid SPolys back into their respective orientations in the shell.
	PolyDebugLevel massZoneLogLevel = PolyDebugLevel::NONE;
	PolyDebugLevel printBoundaryLinesLogLevel = PolyDebugLevel::NONE;			// used for printing categorized lines in all SPolys of a MassZoneBox. (set via debug option, PRINT_BOUNDARY_CATEGORIZED_LINES)
	PolyDebugLevel boundarySPolyConstructionLogLevel = PolyDebugLevel::NONE;	// used for halting and waiting for input, between the construction of boundary SPolys. (set via debug option, HALT_BETWEEN_BOUNDARY_SPOLY_CONSTRUCTION)
	PolyDebugLevel pointClippingLogLevel = PolyDebugLevel::NONE;				// used for output of pont clipping operations. (set via debug option, POINT_CLIPPING
	PolyDebugLevel extractableShellDebugLevel = PolyDebugLevel::NONE;			// will show output of the calls in the function produceExtractableMassZoneShellSPolys
	MassZonePointClipper clipper;
	std::map<int, int> sPolyToSubZoneMap;
	std::map<int, int> subZoneToSPolyMap;
	std::map<int, MeshMatterMeta> meshMatterMetaMap;

	void setMassZoneRecorderRef(ExceptionRecorder* in_massZoneRecorderRef);
	void setMassZoneLogLevel(PolyDebugLevel in_polyDebugLevel);
	void insertSPolyMassSubZone(int in_sPolyID, SPoly in_sPolyCopy);
	void insertSPolyToSubZoneMapEntry(int in_sPolyID, int in_subZoneID);
	void insertSubZoneToSPolyMapEntry(int in_sPolyID, int in_subZoneID);
	void insertDOForClippableSPoly(int in_sPolyID, DebugOption in_debugOption);
	void insertMeshMatterMeta(int in_sPolyID, SPoly* in_massSPolyRef, MassManipulationMode in_originMassManipulationMode);
	void removeMeshMatterMeta(int in_sPolyID);
	void printMeshMatterMeta();
	void printSubZoneSPolys();
	void printBoundaryErrors(MessageContainer* in_messageContainerRef);

	void createMassZoneBoxBoundary(MassZoneBoxType in_massZoneBoxType);
	void setExcludedBoundaries(std::set<BoundaryOrientation> in_excludedBoundaries);	// called by SPolyShellProducer, to send the excluded boundarys to an instance of this class.
	void setLegitimateBoundaryGroupsRef(std::map<BoundaryOrientation, SPolySupergroup>* in_legitBoundarySoupergroupsRef);	// called by SPolyShellProdcer, to set the ref to the legitimate boundary SPoly group map.
	MessageContainer createMassZoneShell(MassZoneType in_massZoneType);
	void enableContestedCategorizedLineAnalysis();
	void wrapperInsertDOForClippableSPoly(int in_sPolyID, DebugOption in_debugOption);
	void insertBoundaryDebugOption(BoundaryOrientation in_BoundaryOrientation, SPolyDO in_sPolyDO);
	void runClipper();
	std::set<BoundaryOrientation> getTouchedBoxFacesList(MassZoneBoxType in_massZoneBoxType);
	void runFirstTertiaryProductionPassInZoneBox(std::set<BoundaryOrientation> in_orientationSet, 
											     std::map<BoundaryOrientation, SPolySupergroup>* in_outputSuperGroupsMapRef);
	void produceExtractableMassZoneShellSPolys(std::map<BoundaryOrientation, SPolySupergroup>* in_outputSuperGroupsMapRef);
	void extractAllBoundarySPolys(std::map<BoundaryOrientation, SPolySupergroup>* in_outputSuperGroupsMapRef);
};

#endif
