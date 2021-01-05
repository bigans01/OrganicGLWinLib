#include "stdafx.h"
#include "MassZone.h"

void MassZone::insertSPolyMassSubZone(int in_sPolyID, SPoly in_sPolyCopy)
{
	//subZoneMap[in_sPolyID].sPolyCopy = in_sPolyCopy;
	int currentSubZoneIndex = subZoneMap.size();
	subZoneMap[currentSubZoneIndex].sPolyCopy = in_sPolyCopy;
	insertMeshMatterMeta(in_sPolyID, &subZoneMap[currentSubZoneIndex].sPolyCopy, subZoneMap[currentSubZoneIndex].sPolyCopy.massManipulationSetting);
	insertSPolyToSubZoneMapEntry(in_sPolyID, currentSubZoneIndex);
}

void MassZone::insertSPolyToSubZoneMapEntry(int in_sPolyID, int in_subZoneID)
{
	sPolyToSubZoneMap[in_sPolyID] = in_subZoneID;
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

void MassZone::createMassZoneBoxBoundary(MassZoneBoxType in_massZoneBoxType)
{
	// the 8 points representing the corners of a MassZoneBox.

	// remember, lower_NW has no offsets applied; upper_SE has all 3 offsets applied.
	glm::vec3 lower_NW;
	glm::vec3 lower_SW;
	glm::vec3 lower_SE;
	glm::vec3 lower_NE;
	glm::vec3 upper_NW;
	glm::vec3 upper_SW;
	glm::vec3 upper_SE;
	glm::vec3 upper_NE;

	// determine the distance between points
	float distanceBetweenPoints = 0.0f;
	if (in_massZoneBoxType == MassZoneBoxType::BLOCK)
	{
		distanceBetweenPoints = 1.0f;
	}
	else if (in_massZoneBoxType == MassZoneBoxType::ENCLAVE)
	{
		distanceBetweenPoints = 4.0f;
	}
	else if (in_massZoneBoxType == MassZoneBoxType::COLLECTION)
	{
		distanceBetweenPoints = 32.0f;
	}

	// set up the values for the points.

	// lower_NW

	// lower_SW
	lower_SW.z += distanceBetweenPoints;

	// lower_SE
	lower_SE.x += distanceBetweenPoints;
	lower_SE.z += distanceBetweenPoints;

	// lower_NE
	lower_NE.x += distanceBetweenPoints;

	// upper_NW
	upper_NW.y += distanceBetweenPoints;

	// upper_SW
	upper_SW.y += distanceBetweenPoints;
	upper_SW.z += distanceBetweenPoints;

	// upper_SE
	upper_SW.x += distanceBetweenPoints;
	upper_SW.y += distanceBetweenPoints;
	upper_SW.z += distanceBetweenPoints;

	// upper_NE
	upper_NE.x += distanceBetweenPoints;
	upper_NE.y += distanceBetweenPoints;

	// POS_X boundary (north) creation/insertion
	MassZoneBoxBoundary northBoundary(lower_NW, upper_NW, upper_NE, lower_NE); 
	zoneBox.insertNewBoundary(MassZoneBoxBoundaryOrientation::POS_X, northBoundary);

	// NEG_X boundary (south) creation/insertion
	MassZoneBoxBoundary southBoundary(lower_SE, upper_SE, upper_SW, lower_SW);
	zoneBox.insertNewBoundary(MassZoneBoxBoundaryOrientation::NEG_X, southBoundary);

	// POS_Z boundary (

}

void MassZone::createMassZoneShell()
{

}