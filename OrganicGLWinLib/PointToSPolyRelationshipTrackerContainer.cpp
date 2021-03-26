#include "stdafx.h"
#include "PointToSPolyRelationshipTrackerContainer.h"

void PointToSPolyRelationshipTrackerContainer::insertRelationshipTrackerData(glm::vec3 in_point, int in_sPolyID, int in_sTriangleID, STriangle* in_sTriangleRef, glm::vec3 in_relatedSPolyEmptyNormal)
{
	bool wasExistingPointFound = false;
	int  existingPointKeyValue = 0;
	auto relationshipTrackerContainerBegin = relationshipTrackerContainer.begin();
	auto relationshipTrackerContainerEnd = relationshipTrackerContainer.end();
	for (; relationshipTrackerContainerBegin != relationshipTrackerContainerEnd; relationshipTrackerContainerBegin++)
	{
		if (relationshipTrackerContainerBegin->second.point == in_point)
		{
			existingPointKeyValue = relationshipTrackerContainerBegin->first;
			wasExistingPointFound = true;
		}
	}

	if (wasExistingPointFound == true)		// insert into an existing record
	{
		relationshipTrackerContainer[existingPointKeyValue].insertPointToSPolyRelationship(in_sPolyID, in_sTriangleID, in_sTriangleRef, in_relatedSPolyEmptyNormal);
	}
	else                                    // insert a new record
	{
		int containerSize = int(relationshipTrackerContainer.size());
		relationshipTrackerContainer[containerSize].point = in_point;			// make sure to set this when inserting a new relationship tracker!
		relationshipTrackerContainer[containerSize].insertPointToSPolyRelationship(in_sPolyID, in_sTriangleID, in_sTriangleRef, in_relatedSPolyEmptyNormal);
	}
}

void PointToSPolyRelationshipTrackerContainer::printRelationshipTrackerData()
{
	auto relationshipTrackerContainerBegin = relationshipTrackerContainer.begin();
	auto relationshipTrackerContainerEnd = relationshipTrackerContainer.end();
	for (; relationshipTrackerContainerBegin != relationshipTrackerContainerEnd; relationshipTrackerContainerBegin++)
	{
		glm::vec3 currentPoint = relationshipTrackerContainerBegin->second.point;
		std::cout << "Tracked Data for point: " << currentPoint.x << ", " << currentPoint.y << ", " << currentPoint.z << std::endl;
		auto shellSPolyBegin = relationshipTrackerContainerBegin->second.relationships.begin();
		auto shellSPolyEnd = relationshipTrackerContainerBegin->second.relationships.end();
		for (; shellSPolyBegin != shellSPolyEnd; shellSPolyBegin++)
		{
			std::cout << "SPoly " << shellSPolyBegin->first << std::endl;
			shellSPolyBegin->second.printSTriangleIndicesAndData();
		}
	}
}

OperableIntSet PointToSPolyRelationshipTrackerContainer::produceRelatedSPolyList()
{
	OperableIntSet returnSet;
	auto relationshipTrackerContainerBegin = relationshipTrackerContainer.begin();
	auto relationshipTrackerContainerEnd = relationshipTrackerContainer.end();
	for (; relationshipTrackerContainerBegin != relationshipTrackerContainerEnd; relationshipTrackerContainerBegin++)
	{
		returnSet += relationshipTrackerContainerBegin->second.generateSPolyIDSet();
	}
	return returnSet;
}

bool PointToSPolyRelationshipTrackerContainer::checkForAnyPointsWithSingleSPoly()
{
	bool singleFound = false;
	auto relationshipTrackerContainerBegin = relationshipTrackerContainer.begin();
	auto relationshipTrackerContainerEnd = relationshipTrackerContainer.end();
	for (; relationshipTrackerContainerBegin != relationshipTrackerContainerEnd; relationshipTrackerContainerBegin++)
	{
		if (relationshipTrackerContainerBegin->second.relationships.size() == 1)
		{
			singleFound = true;
			break;
		}
	}
	return singleFound;
}

PointToSPolyRelationshipTracker* PointToSPolyRelationshipTrackerContainer::fetchSpecificSPolyRelationshipTrackerByPoint(glm::vec3 in_pointToFind)
{
	PointToSPolyRelationshipTracker* returnRef = nullptr;
	auto relationshipTrackerContainerBegin = relationshipTrackerContainer.begin();
	auto relationshipTrackerContainerEnd = relationshipTrackerContainer.end();
	for (; relationshipTrackerContainerBegin != relationshipTrackerContainerEnd; relationshipTrackerContainerBegin++)
	{
		if (relationshipTrackerContainerBegin->second.point == in_pointToFind)
		{
			std::cout << "!! Point found. (" << in_pointToFind.x << ", " << in_pointToFind.y << ", " << in_pointToFind.z << ") " << std::endl;
			returnRef = &relationshipTrackerContainerBegin->second;
		}
	}
	return returnRef;
}

void PointToSPolyRelationshipTrackerContainer::removePointsExistingOutsideOfShell(PolyDebugLevel in_polyDebugLevel)
{
	PolyLogger currentPointLogger;
	currentPointLogger.setDebugLevel(in_polyDebugLevel);
	std::vector<int> removableIDs;
	auto relationshipTrackerContainerBegin = relationshipTrackerContainer.begin();
	auto relationshipTrackerContainerEnd = relationshipTrackerContainer.end();
	for (; relationshipTrackerContainerBegin != relationshipTrackerContainerEnd; relationshipTrackerContainerBegin++)
	{
		bool wasOutsiderFound = false;		// and "outsider" would be any point existing on the same side that an empty normal is pointing in, for any SPoly's PBZ
											// that this point was found as being a part of.
		glm::vec3 pointToCompare = relationshipTrackerContainerBegin->second.point;		// the point to check, of whether or not it is on the interior PBZ side.

		currentPointLogger.log("(PointToSPolyRelationshipTrackerContainer): checking if point ", pointToCompare.x, ", ", pointToCompare.y, ", ", pointToCompare.z, " exists outside of a mass-side PBZ", "\n");

		auto relatedSPolysBegin = relationshipTrackerContainerBegin->second.relationships.begin();	
		auto relatedSPolysEnd = relationshipTrackerContainerBegin->second.relationships.end();	
		for (; relatedSPolysBegin != relatedSPolysEnd; relatedSPolysBegin++)
		{
			// we can use any STriangle in the SPoly, so just get the first.
			glm::vec3 point0 = relatedSPolysBegin->second.sTriangleRelationshipMap.begin()->second.sTriangleRef->triangleLines[0].pointA;	
			glm::vec3 point1 = relatedSPolysBegin->second.sTriangleRelationshipMap.begin()->second.sTriangleRef->triangleLines[1].pointA;
			glm::vec3 point2 = relatedSPolysBegin->second.sTriangleRelationshipMap.begin()->second.sTriangleRef->triangleLines[2].pointA;

			glm::vec3 emptyNormal = relatedSPolysBegin->second.relatedSPolyEmptyNormal;	// we only need to get the first relationship, to get the empty normal.

			wasOutsiderFound = QuatUtils::isPointWithinMassOfPBZ(pointToCompare, point0, point1, point2, emptyNormal, in_polyDebugLevel);
			//if (wasOutsiderFound == true)
			if (wasOutsiderFound == false)
			{
				currentPointLogger.log("(PointToSPolyRelationshipTrackerContainer): This point is an OUTSIDER; will need to be erased.", "\n");
				removableIDs.push_back(relationshipTrackerContainerBegin->first);
				break;	// break out of this for loop, as we are done.
			}
		}
	}

	// if there any ids in the vector, remove them appropriate ID from the container.
	auto removalVectorBegin = removableIDs.begin();
	auto removalVectorEnd = removableIDs.end();
	for (; removalVectorBegin != removalVectorEnd; removalVectorBegin++)
	{
		relationshipTrackerContainer.erase(*removalVectorBegin);
	}
}

void PointToSPolyRelationshipTrackerContainer::runPointsAgainstShellSlices(PolyDebugLevel in_polyDebugLevel, std::map<int, SPoly*> in_clippingShellMapCopy)
{
	PolyLogger currentPointLogger;
	currentPointLogger.setDebugLevel(in_polyDebugLevel);
	std::vector<int> removableIDs;

	PointToMassTrial massTrial;
	auto relationshipTrackerContainerBegin = relationshipTrackerContainer.begin();
	auto relationshipTrackerContainerEnd = relationshipTrackerContainer.end();
	for (; relationshipTrackerContainerBegin != relationshipTrackerContainerEnd; relationshipTrackerContainerBegin++)
	{
		bool wasOutsiderFound = false;
		glm::vec3 pointToCompare = relationshipTrackerContainerBegin->second.point;

		// Loop 1: determine the relationships; if wasOutsiderFound = false, then we are WITHIN_MASS; if an outsider is found, the point is NOT_WITHIN_MASS.


		// Loop 2: for each SPoly that a point was found as being within the PBZ of, we must run the specific STriangle in that SPoly through a ShellSlice.
		auto relatedSPolysBegin = relationshipTrackerContainerBegin->second.relationships.begin();
		auto relatedSPolysEnd = relationshipTrackerContainerBegin->second.relationships.end();
		for (; relatedSPolysBegin != relatedSPolysEnd; relatedSPolysBegin++)
		{
			// for the current SPoly we're analyzing for the point, we must pass the following into a ShellSlice constructor:
			//
			// -a reference to the STriangle in the SPoly we're looking at in relatedSPolyBegin
			// -get the ID of the SPoly that we're looking at the PBZ of, and subtract it from a copy of the in_clippingShellMap. 
			// -get the empty normal of this SPoly.
			// -the point that we're comparing

			STriangle* targetSTriangleRef = relatedSPolysBegin->second.sTriangleRelationshipMap.begin()->second.sTriangleRef;	// get the STriangle of the related SPoly.
			int shellSliceSPolyID = relatedSPolysBegin->first;			// the ID of the SPoly that we're doing a ShellSlice for.
			auto shellSliceclippingShellMapCopy = in_clippingShellMapCopy;		// create a copy, of all the shells in the map...
			shellSliceclippingShellMapCopy.erase(shellSliceSPolyID);					// ...and erase the ID of the SPoly that we're doing a shell slice for from it
			glm::vec3 relatedSPolyEmptyNormal = relatedSPolysBegin->second.relatedSPolyEmptyNormal;	// we need the empty normal of the SPoly we're doing a shell slice for

			// insert a new MassZoneShellSlice into the appropriate PointToMassRelationshipJudge, by using the current value of pointToCompare.
			massTrial.insertShellSlice(pointToCompare, relatedSPolysBegin->first, targetSTriangleRef, relatedSPolyEmptyNormal, shellSliceclippingShellMapCopy);
		}
	}

	// optional: print the judges of the PointToMassTrial
	if (currentPointLogger.isLoggingSet())
	{
		currentPointLogger.log("(PointTOSPolyRelationshipTrackerContainer): printing judge data...", "\n");
		massTrial.printJudgeMetaData();
	}

	// run the judgements.
	massTrial.executeAllJudgements();

	// if there any ids in the vector, remove them appropriate ID from the container.
	auto removalVectorBegin = removableIDs.begin();
	auto removalVectorEnd = removableIDs.end();
	for (; removalVectorBegin != removalVectorEnd; removalVectorBegin++)
	{
		relationshipTrackerContainer.erase(*removalVectorBegin);
	}
}