#include "stdafx.h"
#include "MassZonePointClipper.h"

void MassZonePointClipper::insertSPolySuperGroupRefsIntoClippingShell(SPolySupergroup* in_sPolySuperGroupRef)
{
	auto superGroupSPolysBegin = in_sPolySuperGroupRef->sPolyMap.begin();
	auto superGroupSPolysEnd = in_sPolySuperGroupRef->sPolyMap.end();
	for (; superGroupSPolysBegin != superGroupSPolysEnd; superGroupSPolysBegin++)
	{
		std::cout << ":: Inserted SPoly from super group into clipper. " << std::endl;
		int currentMapSize = clippingShellMap.size();
		clippingShellMap[currentMapSize] = &superGroupSPolysBegin->second;
	}
}

void MassZonePointClipper::insertSPolyRefIntoClippingShell(SPoly* in_sPolyRef)
{
	int currentMapSize = clippingShellMap.size();
	clippingShellMap[currentMapSize] = in_sPolyRef;
}

void MassZonePointClipper::setOtherZoneMeshMatterMetaMapRef(std::map<int, MeshMatterMeta>* in_otherZoneMeshMatterMetaMapRef)
{
	otherZoneMeshMatterMetaMapRef = in_otherZoneMeshMatterMetaMapRef;
}

void MassZonePointClipper::run()
{
	std::cout << "!! Size of clipping shell map: " << clippingShellMap.size() << std::endl;
	std::cout << "!! Size of other zone's mesh matter map: " << otherZoneMeshMatterMetaMapRef->size() << std::endl;

	auto otherMeshMatterMapBegin = otherZoneMeshMatterMetaMapRef->begin();
	auto otherMeshMatterMapEnd = otherZoneMeshMatterMetaMapRef->end();
	for (; otherMeshMatterMapBegin != otherMeshMatterMapEnd; otherMeshMatterMapBegin++)
	{
		compareMeshMatterMetaAgainstClippingShells(&otherMeshMatterMapBegin->second);
	}
}

void MassZonePointClipper::compareMeshMatterMetaAgainstClippingShells(MeshMatterMeta* in_meshMatterMetaRef)
{
	SPoly* currentMeshMatterSPoly = in_meshMatterMetaRef->massSPolyRef;
	PointToMassRelationshipMap currentRelationshipMap = currentMeshMatterSPoly->generatePointToMassRelationshipMap();
	PointToSPolyRelationshipTrackerContainer relationshipTrackerContainer;

	// Phase 1: run all points of the SPoly (which will be in the currentRelationshipMap) through all SPolys in the clippingShellMap, to see which
	// fields we must compare the points to.
	auto currentPointToAnalyzeBegin = currentRelationshipMap.relationshipMap.begin();
	auto currentPointToAnalyzeEnd = currentRelationshipMap.relationshipMap.end();
	for (; currentPointToAnalyzeBegin != currentPointToAnalyzeEnd; currentPointToAnalyzeBegin++)
	{
		glm::vec3 pointToCompareFor = currentPointToAnalyzeBegin->second.point;
		auto clippingShellMapBegin = clippingShellMap.begin();
		auto clippingShellMapEnd = clippingShellMap.end();
		for (; clippingShellMapBegin != clippingShellMapEnd; clippingShellMapBegin++)
		{
			SPoly* currentClippingShellSPolyRef = clippingShellMapBegin->second;	// get a ref to the shell SPoly.
			int numberOfSTriangles = currentClippingShellSPolyRef->triangles.size();
			for (int x = 0; x < numberOfSTriangles; x++)
			{
				STriangle* currentSTriangleRef = &currentClippingShellSPolyRef->triangles[x];

				// only insert new relationship tracker data when the pointToCompareFor is found as being with the STriangle's planarBoundedZone (??? may need a better name for that!)
				// ...
				// use a new QMBool machine to do this, get the bool result, insert if true.
				// ...

				//relationshipTrackerContainer.insertRelationshipTrackerData(pointToCompareFor, clippingShellMapBegin->first, x, currentSTriangleRef);
			}
		}
	}

	// Phase 2: check if the currentMeshMatterSPoly is coplanar to any of the SPolys that are registered in the relationshipTrackerContainer; if it is, 
	// we're done -- because the clipper can't handle cases where the currentMeshMatterSPoly is coplanar to one of the PBZs (planarBoundedZone) that is for a
	// STriangle that's part of an SPoly we're comparing to.

}