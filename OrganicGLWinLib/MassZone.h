#pragma once

#ifndef MASSZONE_H
#define MASSZONE_H

#include "MassSubZone.h"
#include "MeshMatterMeta.h"
#include <map>


class MassZone
{
public:
	std::map<int, MassSubZone> subZoneMap;
	std::map<int, MeshMatterMeta> meshMatterMetaMap;
	void insertMassSubZone(int in_sPolyID, SPoly in_sPolyCopy);
	void insertMeshMatterMeta(int in_sPolyID, SPoly* in_massSPolyRef, MassManipulationMode in_originMassManipulationMode);
	void removeMeshMatterMeta(int in_sPolyID);
	void printMeshMatterMeta();
private:

};

#endif
