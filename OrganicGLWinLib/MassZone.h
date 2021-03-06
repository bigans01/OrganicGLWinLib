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
#include "MassZoneBoxBoundaryOrientation.h"
#include "MassZoneType.h"
#include "MassZonePointClipper.h"
#include "DebugOption.h"



class MassZone
{
private:
	friend class MassZoneMaster;
	friend class SPolySet;
	friend class BorderSPolyProducer;
	MassZoneBox zoneBox;

	std::map<int, MassSubZone> subZoneMap;
	std::map<MassZoneBoxBoundaryOrientation, SPolyDOSet> boundaryDebugOptions;
	std::set<MassZoneBoxBoundaryOrientation> boxFaceProductionPermits;
	PolyDebugLevel massZoneLogLevel = PolyDebugLevel::NONE;
	PolyDebugLevel printBoundaryLinesLogLevel = PolyDebugLevel::NONE;			// used for printing categorized lines in all SPolys of a MassZoneBox. (set via debug option, PRINT_BOUNDARY_CATEGORIZED_LINES)
	PolyDebugLevel boundarySPolyConstructionLogLevel = PolyDebugLevel::NONE;	// used for halting and waiting for input, between the construction of boundary SPolys. (set via debug option, HALT_BETWEEN_BOUNDARY_SPOLY_CONSTRUCTION)
	PolyDebugLevel pointClippingLogLevel = PolyDebugLevel::NONE;				// used for output of pont clipping operations. (set via debug option, POINT_CLIPPING
	MassZonePointClipper clipper;
	std::map<int, int> sPolyToSubZoneMap;
	std::map<int, int> subZoneToSPolyMap;
	std::map<int, MeshMatterMeta> meshMatterMetaMap;

	void setMassZoneLogLevel(PolyDebugLevel in_polyDebugLevel);
	void insertSPolyMassSubZone(int in_sPolyID, SPoly in_sPolyCopy);
	void insertSPolyToSubZoneMapEntry(int in_sPolyID, int in_subZoneID);
	void insertSubZoneToSPolyMapEntry(int in_sPolyID, int in_subZoneID);
	void insertDOForClippableSPoly(int in_sPolyID, DebugOption in_debugOption);
	void insertMeshMatterMeta(int in_sPolyID, SPoly* in_massSPolyRef, MassManipulationMode in_originMassManipulationMode);
	void removeMeshMatterMeta(int in_sPolyID);
	void printMeshMatterMeta();
	void createMassZoneBoxBoundary(MassZoneBoxType in_massZoneBoxType);
	void createMassZoneShell(MassZoneType in_massZoneType);
	void enableContestedCategorizedLineAnalysis();
	void wrapperInsertDOForClippableSPoly(int in_sPolyID, DebugOption in_debugOption);
	void insertBoundaryDebugOption(MassZoneBoxBoundaryOrientation in_massZoneBoxBoundaryOrientation, SPolyDO in_sPolyDO);
	void runClipper();
	std::set<MassZoneBoxBoundaryOrientation> getTouchedBoxFacesList(MassZoneBoxType in_massZoneBoxType);
	void runFirstTertiaryProductionPassInZoneBox(std::set<MassZoneBoxBoundaryOrientation> in_orientationSet, 
											     std::map<MassZoneBoxBoundaryOrientation, SPolySupergroup>* in_outputSuperGroupsMapRef);
	void produceExtractableMassZoneShellSPolys(std::map<MassZoneBoxBoundaryOrientation, SPolySupergroup>* in_outputSuperGroupsMapRef);
};

#endif
