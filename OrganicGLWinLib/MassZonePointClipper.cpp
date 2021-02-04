#include "stdafx.h"
#include "MassZonePointClipper.h"

void MassZonePointClipper::insertSPolySuperGroupRefsIntoClippingShell(SPolySupergroup* in_sPolySuperGroupRef)
{
	auto superGroupSPolysBegin = in_sPolySuperGroupRef->sPolyMap.begin();
	auto superGroupSPolysEnd = in_sPolySuperGroupRef->sPolyMap.end();
	for (; superGroupSPolysBegin != superGroupSPolysEnd; superGroupSPolysBegin++)
	{
		//std::cout << ":: Inserted SPoly from super group into clipper. " << std::endl;
		clipperPolyLogger.log("(MassZonePointClipper) ", zoneString," :: Inserted SPoly from super group into clipper. ", "\n");
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
	//std::cout << "!! Size of clipping shell map: " << int(clippingShellMap.size()) << std::endl;
	//std::cout << "!! Size of other zone's mesh matter map: " << int(otherZoneMeshMatterMetaMapRef->size()) << std::endl;

	clipperPolyLogger.log("(MassZonePointClipper) ", zoneString," !! Size of clipping shell map: ", int(clippingShellMap.size()), "\n");
	clipperPolyLogger.log("(MassZonePointClipper) ", zoneString," !! Size of other zone's mesh matter map: ", int(otherZoneMeshMatterMetaMapRef->size()), "\n");
	
	auto otherMeshMatterMapBegin = otherZoneMeshMatterMetaMapRef->begin();
	auto otherMeshMatterMapEnd = otherZoneMeshMatterMetaMapRef->end();
	for (; otherMeshMatterMapBegin != otherMeshMatterMapEnd; otherMeshMatterMapBegin++)
	{
		if (compareMeshMatterMetaAgainstClippingShells(&otherMeshMatterMapBegin->second) == true)	// if it was determined that it needs to be purged, put it into sPolysToPurgeSet.
		{
			//std::cout << "!!! Flagging SPoly with ID " << otherMeshMatterMapBegin->second.referencedSPolyID << " as purgable." << std::endl;
			clipperPolyLogger.log("(MassZonePointClipper) ", zoneString," !!! Flagging SPoly with ID ", otherMeshMatterMapBegin->second.referencedSPolyID, " as purgable.", "\n");
			sPolysToPurge.insert(otherMeshMatterMapBegin->second.referencedSPolyID);
		}
	}
}

bool MassZonePointClipper::compareMeshMatterMetaAgainstClippingShells(MeshMatterMeta* in_meshMatterMetaRef)
{
	bool willBePurged = false;
	SPoly* currentMeshMatterSPoly = in_meshMatterMetaRef->massSPolyRef;
	//std::cout << "|||||||||||||||||| Running SPoly, with SPolySet ID of " << in_meshMatterMetaRef->referencedSPolyID << std::endl;
	clipperPolyLogger.log("(MassZonePointClipper) ", zoneString," |||||||||||||||||| Running SPoly, with SPolySet ID of ", in_meshMatterMetaRef->referencedSPolyID, "\n");

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


	// Phase 2: Three checks. 
	
	// CHECK 2.1: if the currentMeshMatterSPoly is coplanar to any of the SPolys that are registered in the relationshipTrackerContainer; if it is,
	// we are done. The reasons for this are as follows:
	// 
	// 1.) If the currentMeshMatterSPoly exists, it means it didn't have any CleaveSequences to process. However, it also means that there is a chance that it 
	//     could be coplanar to at least one compared-to SPoly that exists in the relationshipTrackerContainer.
	// 2.) If #1 holds true, in that this SPoly is coplanar to a compared-to SPoly in the relationshipTrackerContainer, then it means that the MassZonePointClipper
	//     must disregard this comparison of the currentMeshMatterSPoly to those in the relatonshipTrackerContainer.
	//
	// The reasoning for disregarding when #2 is met, is due to the fact that the MassZonePointClipper's logic isn't designed to work with a SPoly that is coplanar
	// to any of the SPolys in the relationshipTrackerContainer. 
	// 
	// CHECK 2.2: check to see if the number of points contained within the relationshipTrackerContainer is equal to the number of points in the SPoly. If the number 
	// within the container is less than the number in the SPoly, we are done. The reason for this is that it means at least one point was found as not
	// being within any of the PBZs of the shell; this guarantees that it is impossible for the currentMeshMatterSPoly to be clipped, since it isn't entirely
	// contained within the shell.
	//
	// CHECK 2.3: For points within the relationshipTrackerContainer, all points must exist within the PBZ of at least 2 SPolys. We are done if we detect any points
	// as only being within one SPoly.
	
	// CHECK 2.1
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
			//std::cout << "!!!! Notice, coplanarity detected for mesh matter SPoly with ID: " << in_meshMatterMetaRef->referencedSPolyID << std::endl;
			clipperPolyLogger.log("(MassZonePointClipper) ", zoneString," !!!! Notice, coplanarity detected for mesh matter SPoly with ID: ", in_meshMatterMetaRef->referencedSPolyID, "\n");
			break;
		}
	}

	// CHECK 2.2
	bool areAllPointsWithinShell = false;		// if all points of the SPoly are somewhere in the combined PBZ of the shell, this will get set to true.
	if (relationshipTrackerContainer.relationshipTrackerContainer.size() == currentMeshMatterSPoly->borderLines.size())
	{
		areAllPointsWithinShell = true;
	}

	// for printing out the contents of the trackerContainer (debug only)
	if 
    (	
		(foundAsBeingCoplanar == false)
		&&
		(areAllPointsWithinShell == true)
	)
	{
		// CHECK 2.3
		if (relationshipTrackerContainer.checkForAnyPointsWithSingleSPoly() == false)
		{

			relationshipTrackerContainer.printRelationshipTrackerData();
			BorderLineLinkContainer linkContainer = currentMeshMatterSPoly->buildBuildBorderLineLinkContainer();
			//std::cout << "!!! Finished building BorderLineLinkContainer." << std::endl;
			clipperPolyLogger.log("(MassZonePointClipper) ", zoneString," !!! Finished building BorderLineLinkContainer.", "\n");
			if (runFirstTwoDisqualificationPasses(&linkContainer, &relationshipTrackerContainer) == true)
			{
				clipperPolyLogger.log("(MassZonePointClipper) ", zoneString," !!! SPoly flagged as being purgable.", "\n");
				willBePurged = true;
			}
		}
		else
		{
			//std::cout << "!! Notice: at least one point was detected as having only one SPoly-PBZ relationship; discontinuing." << std::endl;
			clipperPolyLogger.log("(MassZonePointClipper) ", zoneString,"!! Notice: at least one point was detected as having only one SPoly-PBZ relationship; discontinuing.", "\n");
		}
	}
	
	if (clipperPolyLogger.isLoggingSet() == true)
	{
		clipperPolyLogger.log("(MassZonePointClipper) ", zoneString," !! Finished purging analysis.", "\n");
		clipperPolyLogger.waitForDebugInput();
	}

	return willBePurged;
}

void MassZonePointClipper::setClipperDebugLevel(PolyDebugLevel in_polyDebugLevel, std::string in_zoneString)
{
	clipperPolyLogger.setDebugLevel(in_polyDebugLevel);
	clipperPolyLoggerDebugLevel = in_polyDebugLevel;
	zoneString = in_zoneString;
}

bool MassZonePointClipper::runFirstTwoDisqualificationPasses(BorderLineLinkContainer* in_borderLineLinkContainerRef, PointToSPolyRelationshipTrackerContainer* in_trackerContainerRef)
{
	bool isPurgable = false;		// set this to true if all tests pass.

	// first pass: check for any points which are coplanar to an SPoly that is within the shell
	auto linksBegin = in_borderLineLinkContainerRef->linkMap.begin();
	auto linksEnd = in_borderLineLinkContainerRef->linkMap.end();
	for (; linksBegin != linksEnd; linksBegin++)
	{
		bool pointFoundAsCoplanar = false;	// set to true when the point as being coplanar to any STriangle.
		PointToSPolyRelationshipTracker* trackerRef = in_trackerContainerRef->fetchSpecificSPolyRelationshipTrackerByPoint(linksBegin->second.linkPoint);
		auto trackerSPolysBegin = trackerRef->relationships.begin();
		auto trackerSPolysEnd = trackerRef->relationships.end();
		for (; trackerSPolysBegin != trackerSPolysEnd; trackerSPolysBegin++)
		{
			auto currentSPolySTriangleBegin = trackerSPolysBegin->second.sTriangleRelationshipMap.begin();
			auto currentSPolySTriangleEnd = trackerSPolysBegin->second.sTriangleRelationshipMap.end();
			for (; currentSPolySTriangleBegin != currentSPolySTriangleEnd; currentSPolySTriangleBegin++)
			{
				STriangle* currentTriangleToCompareTo = currentSPolySTriangleBegin->second.sTriangleRef;
				if (clipperPolyLogger.isLoggingSet() == true)
				{
					clipperPolyLogger.log("(MassZonePointClipper) ", zoneString, " Points of triangle to compare to: ", "\n");
					for (int x = 0; x < 3; x++)
					{
						clipperPolyLogger.log("(MassZonePointClipper) ", zoneString, " [", x, "]: ", currentTriangleToCompareTo->triangleLines[x].pointA.x, ", ", currentTriangleToCompareTo->triangleLines[x].pointA.y, ", ", currentTriangleToCompareTo->triangleLines[x].pointA.z, "\n");
					}

				}

				// We must check if either of the border lines which are part of the BorderLineLink we're looking at, are on the same plane as
				// any of the STriangles for which this point is found as being within that STriangle's PBZ.

				// *********************************************************First border line 
				SPolyBorderLines* firstBorderLineInLinkRef = linksBegin->second.linkedBorderLines[0];
				STriangleLine tempLine;
				tempLine.pointA = firstBorderLineInLinkRef->pointA;
				tempLine.pointB = firstBorderLineInLinkRef->pointB;
				clipperPolyLogger.log("(MassZonePointClipper) ", zoneString, " (First line) Comparing line with points (", tempLine.pointA.x, ", ", tempLine.pointA.y, ", ", tempLine.pointA.z, " | ",
					tempLine.pointB.x, ", ", tempLine.pointB.y, ", ", tempLine.pointB.z, ") ", "\n");
				FusionCandidateProducer shellProducerFirst(PolyDebugLevel::NONE);
				FusionCandidate shellCandidateFirst = shellProducerFirst.produceCandidate(*currentTriangleToCompareTo, tempLine);		// send a copy of the STriangle
				if
				(
					(shellCandidateFirst.candidateIntersectionResult.wasIntersectFound == 1)
					&&
					(shellCandidateFirst.candidateIntersectionResult.intersectedPoint == tempLine.pointB)
				)
				{
					clipperPolyLogger.log("(MassZonePointClipper) ", zoneString, "(First Line) (( NOTICE )) : point ", tempLine.pointB.x, ", ", tempLine.pointB.y, ", ", tempLine.pointB.z,
					" found as being coplanar, breaking.", "\n");
					in_borderLineLinkContainerRef->updateLinkPointStatus(tempLine.pointB, BorderLineLinkPointState::COPLANAR);
					pointFoundAsCoplanar = true;
					break;
				}

				// *********************************************************Second border line 
				SPolyBorderLines* secondBorderLineInLinkRef = linksBegin->second.linkedBorderLines[1];
				STriangleLine tempLineB;
				tempLineB.pointA = secondBorderLineInLinkRef->pointA;
				tempLineB.pointB = secondBorderLineInLinkRef->pointB;
				clipperPolyLogger.log("(MassZonePointClipper) ", zoneString, " (Second line) Comparing line with points (", tempLineB.pointA.x, ", ", tempLineB.pointA.y, ", ", tempLineB.pointA.z, " | ",
					tempLineB.pointB.x, ", ", tempLineB.pointB.y, ", ", tempLineB.pointB.z, ") ", "\n");
				FusionCandidateProducer shellProducerSecond(PolyDebugLevel::NONE);
				FusionCandidate shellCandidateSecond = shellProducerSecond.produceCandidate(*currentTriangleToCompareTo, tempLineB);
				if
				(
					(shellCandidateSecond.candidateIntersectionResult.wasIntersectFound == 1)
					&&
					(shellCandidateSecond.candidateIntersectionResult.intersectedPoint == tempLineB.pointA)
				)
				{
					clipperPolyLogger.log("(MassZonePointClipper) ", zoneString, "(Second Line) (( NOTICE )) : point ", tempLineB.pointA.x, ", ", tempLineB.pointA.y, ", ", tempLineB.pointA.z,
						" found as being coplanar, breaking.", "\n");
					in_borderLineLinkContainerRef->updateLinkPointStatus(tempLineB.pointA, BorderLineLinkPointState::COPLANAR);
					pointFoundAsCoplanar = true;
					break;
				}

			}

			// as soon as a point in the first pass is determined as being coplanar to any STriangle that it is related to,
			// we should stop, as continuing on is pointless. 
			if (pointFoundAsCoplanar == true)		//if we broke out of the previous loop, break out of this one too, and we'll be done.
			{
				break;
			}
		}
	}
	if (in_borderLineLinkContainerRef->checkIfAllLinksPointsAreCoplanar() == true)
	{
		clipperPolyLogger.log("(MassZonePointClipper) ", zoneString," FLAGGED AS PURGABLE -> This SPoly has all it's point set to COPLANAR; the SPoly is now purgable; this function will now halt and return TRUE.", "\n");
		isPurgable = true;
	}

	//std::cout << "!! Finished passes for this SPoly. " << std::endl;

	return isPurgable;
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