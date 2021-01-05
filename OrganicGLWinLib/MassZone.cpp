#include "stdafx.h"
#include "MassZone.h"

void MassZone::insertMassSubZone(int in_sPolyID, SPoly in_sPolyCopy)
{
	subZoneMap[in_sPolyID].sPolyCopy = in_sPolyCopy;
	insertMeshMatterMeta(in_sPolyID, &subZoneMap[in_sPolyID].sPolyCopy, subZoneMap[in_sPolyID].sPolyCopy.massManipulationSetting);
}

void MassZone::insertMeshMatterMeta(int in_sPolyID, SPoly* in_massSPolyRef, MassManipulationMode in_originMassManipulationMode)
{
	MeshMatterMeta newMeta(in_massSPolyRef, in_originMassManipulationMode);
	meshMatterMetaMap[in_sPolyID] = newMeta;
}

void MassZone::removeMeshMatterMeta(int in_sPolyID)
{
	meshMatterMetaMap.erase(in_sPolyID);
}

void MassZone::printMeshMatterMeta()
{
	auto meshMatterMetaBegin = meshMatterMetaMap.begin();
	auto meshMatterMetaEnd = meshMatterMetaMap.end();
	for (; meshMatterMetaBegin != meshMatterMetaEnd; meshMatterMetaBegin++)
	{
		std::cout << "SPolyID: " << meshMatterMetaBegin->first << std::endl;
	}
}