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
	std::cout << "|||||||||||||||||| Running SPoly, with SPolySet ID of " << in_meshMatterMetaRef->referencedSPolyID << std::endl;

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
				
				if (checkIfPointIsWithinPBZ(pointToCompareFor, *currentSTriangleRef) == true)
				{
					relationshipTrackerContainer.insertRelationshipTrackerData(pointToCompareFor, clippingShellMapBegin->first, x, currentSTriangleRef);
				}
				
				// ...
			}
		}
	}


	// Phase 2: check if the currentMeshMatterSPoly is coplanar to any of the SPolys that are registered in the relationshipTrackerContainer; if it is,
	// we are done. The reasons for this are as follows:
	// 
	// 1.) If the currentMeshMatterSPoly exists, it means it didn't have any CleaveSequences to process. However, it also means that there is a chance that it 
	//     could be coplanar to at least one compared-to SPoly that exists in the relationshipTrackerContainer.
	// 2.) If #1 holds true, in that this SPoly is coplanar to a compared-to SPoly in the relationshipTrackerContainer, then it means that the MassZonePointClipper
	//     must disregard this comparison of the currentMeshMatterSPoly to those in the relatonshipTrackerContainer.
	//
	// The reasoning for disregarding when #2 is met, is due to the fact that the MassZonePointClipper's logic isn't designed to work with a SPoly that is coplanar
	// to any of the SPolys in the relationshipTrackerContainer. 
	bool foundAsBeingCoplanar = false;
	OperableIntSet sPolysToCompareAgainst = relationshipTrackerContainer.produceRelatedSPolyList();
	auto compareAgainstBegin = sPolysToCompareAgainst.intSet.begin();
	auto compareAgainstEnd = sPolysToCompareAgainst.intSet.end();
	for (; compareAgainstBegin != compareAgainstEnd; compareAgainstBegin++)
	{
		int comparedToIndex = *compareAgainstBegin;
		auto foundSPolyIterator = clippingShellMap.find(comparedToIndex);
		CoplanarChecker checker(currentMeshMatterSPoly, foundSPolyIterator->second, PolyDebugLevel::NONE);
		if (checker.coplanarityDetected == true)
		{
			foundAsBeingCoplanar = true;
			std::cout << "!!!! Notice, coplanarity detected for mesh matter SPoly with ID: " << in_meshMatterMetaRef->referencedSPolyID << std::endl;
			break;
		}
	}

	// for printing out the contents of the trackerContainer (debug only)
	if (foundAsBeingCoplanar == false)
	{
		relationshipTrackerContainer.printRelationshipTrackerData();
	}
	std::cout << "!!! Finished printing relationshipTrackerContainer contents. Enter number to continue. " << std::endl;
	int someVal = 3;
	std::cin >> someVal;
}

bool MassZonePointClipper::checkIfPointIsWithinPBZ(glm::vec3 in_pointToCheck, STriangle in_sTriangleCopy)
{
	bool isWithinPBZ = false;

	glm::vec3 pointToCheckCopy = in_pointToCheck;
	STriangle sTriangleCopy = in_sTriangleCopy;

	QuatRotationPoints points;
	points.pointsRefVector.push_back(&pointToCheckCopy);
	points.pointsRefVector.push_back(&sTriangleCopy.triangleLines[0].pointA);
	points.pointsRefVector.push_back(&sTriangleCopy.triangleLines[1].pointA);
	points.pointsRefVector.push_back(&sTriangleCopy.triangleLines[2].pointA);
	QMBoolPointWithinTrianglePBZ pointSolver;
	isWithinPBZ = pointSolver.solve(&points, PolyDebugLevel::NONE);

	/*
	if (isWithinPBZ == true)
	{
		std::cout << "!!! Point " << in_pointToCheck.x << ", " << in_pointToCheck.y << ", " << in_pointToCheck.z << " is within PBZ; " << std::endl;
		std::cout << "Triangle PBZ points are: " << std::endl;
		for (int x = 0; x < 3; x++)
		{
			std::cout << x << ": " << in_sTriangleCopy.triangleLines[x].pointA.x << ", " << in_sTriangleCopy.triangleLines[x].pointA.y << ", " << in_sTriangleCopy.triangleLines[x].pointA.z << std::endl;
		}
		std::cout <<  "Enter number to continue..." << std::endl;
		int someVal = 3;
		std::cin >> someVal;
	}
	*/

	return isWithinPBZ;
}