#pragma once

#ifndef MASSZONE_H
#define MASSZONE_H

#include "MassZoneBox.h"
#include "MassSubZone.h"
#include "MeshMatterMeta.h"
#include "MassZoneBoxType.h"
#include <map>
#include <glm/glm.hpp>


class MassZone
{
public:
	MassZoneBox zoneBox;
	void insertSPolyMassSubZone(int in_sPolyID, SPoly in_sPolyCopy);
	void insertSPolyToSubZoneMapEntry(int in_sPolyID, int in_subZoneID);
	void insertMeshMatterMeta(int in_sPolyID, SPoly* in_massSPolyRef, MassManipulationMode in_originMassManipulationMode);
	void removeMeshMatterMeta(int in_sPolyID);
	void printMeshMatterMeta();
	void createMassZoneBoxBoundary(MassZoneBoxType in_massZoneBoxType);
	void createMassZoneShell();
	std::map<int, MassSubZone> subZoneMap;
private:
	std::map<int, int> sPolyToSubZoneMap;
	std::map<int, MeshMatterMeta> meshMatterMetaMap;

};

#endif
