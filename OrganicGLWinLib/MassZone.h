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



class MassZone
{
public:
	friend class MassZoneMaster;
	MassZoneBox zoneBox;
	void setMassZoneLogLevel(PolyDebugLevel in_polyDebugLevel);
	void insertSPolyMassSubZone(int in_sPolyID, SPoly in_sPolyCopy);
	void insertSPolyToSubZoneMapEntry(int in_sPolyID, int in_subZoneID);
	void insertSubZoneToSPolyMapEntry(int in_sPolyID, int in_subZoneID);
	void insertMeshMatterMeta(int in_sPolyID, SPoly* in_massSPolyRef, MassManipulationMode in_originMassManipulationMode);
	void removeMeshMatterMeta(int in_sPolyID);
	void printMeshMatterMeta();
	void createMassZoneBoxBoundary(MassZoneBoxType in_massZoneBoxType);
	void createMassZoneShell();
	std::map<int, MassSubZone> subZoneMap;
	std::map<MassZoneBoxBoundaryOrientation, SPolyDOSet> boundaryDebugOptions;
	void insertBoundaryDebugOption(MassZoneBoxBoundaryOrientation in_massZoneBoxBoundaryOrientation, SPolyDO in_sPolyDO);
private:
	PolyDebugLevel massZoneLogLevel = PolyDebugLevel::NONE;
	PolyDebugLevel printBoundaryLinesLogLevel = PolyDebugLevel::NONE;			// used for printing categorized lines in all SPolys of a MassZoneBox. (set via debug option)
	PolyDebugLevel boundarySPolyConstructionLogLevel = PolyDebugLevel::NONE;	// used for halting and waiting for input, between the construction of boundary SPolys.
	std::map<int, int> sPolyToSubZoneMap;
	std::map<int, int> subZoneToSPolyMap;
	std::map<int, MeshMatterMeta> meshMatterMetaMap;

};

#endif
