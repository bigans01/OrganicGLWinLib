#include "stdafx.h"
#include "CoplanarRelationshipTracker.h"

void CoplanarRelationshipTracker::insertCoplanarRelationship(int in_trackedSPolyID, SPoly* in_trackedSPolyRef, int in_relatedSPolyID, SPoly* in_relatedSPolyRef)
{
	if (in_trackedSPolyRef->massManipulationSetting == MassManipulationMode::CREATION)
	{
		insertRelationship(in_trackedSPolyID, in_trackedSPolyRef, in_relatedSPolyID, in_relatedSPolyRef);
	}
	else if (in_trackedSPolyRef->massManipulationSetting == MassManipulationMode::DESTRUCTION)
	{
		// for a destructive relationship, the parameters to use for the tracked/related SPolys are reversed.
		// In other words, the tracked SPoly passed into this function becomes the related SPoly, and the related SPoly becomes the tracked SPoly.
		insertRelationship(in_relatedSPolyID, in_relatedSPolyRef, in_trackedSPolyID, in_trackedSPolyRef);
	}

	//std::cout << "!! Verifying pointers are OK: " << std::endl;
	//std::cout << "-> tracked SPoly, number of border lines: " << relationshipContainer[in_trackedSPolyID].trackedSPolyRef->numberOfBorderLines << std::endl;
	//std::cout << "-> tracked SPoly, number of border lines: " << relationshipContainer[in_trackedSPolyID].trackedSPolyRef.numberOfBorderLines << std::endl;
	//std::cout << "-> related SPoly, number of border lines: " << relationshipContainer[in_trackedSPolyID].relationshipMap.refMap[in_relatedSPolyID]->numberOfBorderLines << std::endl;
	//std::cout << "-> related SPoly, number of border lines: " << relationshipContainer[in_trackedSPolyID].relationshipMap.refMap[in_relatedSPolyID].numberOfBorderLines << std::endl;
	
}

void CoplanarRelationshipTracker::runAllCuttingSequenceTests()
{
	
	auto relationshipsBegin = relationshipContainer.begin();
	auto relationshipsEnd = relationshipContainer.end();
	for (; relationshipsBegin != relationshipsEnd; relationshipsBegin++)
	{
		std::cout << "-> Found CoplanarRelationship, for tracked SPoly -> " << relationshipsBegin->first << std::endl;
		applyCoplanarRelationshipDebugFlagsIfFound(relationshipsBegin->first);
		bool didSPolyPassCuttingTests = relationshipsBegin->second.rotateToXYPlaneAndRunCuttingSequenceTests();
		if (didSPolyPassCuttingTests == false)
		{
			removableSPolys.intSet.insert(relationshipsBegin->first);
		}
	}
	
}

void CoplanarRelationshipTracker::setRelationshipTrackerDebugLevel(PolyDebugLevel in_polyDebugLevel)
{
	relationshipTrackerLogger.setDebugLevel(in_polyDebugLevel);
	relationshipTrackerDebugLevel = in_polyDebugLevel;
}

void CoplanarRelationshipTracker::insertDOForSpecificTrackedSPoly(int in_trackedSPolyID, DebugOption in_debugOption)
{
	//specificRelationshipOptions[in_trackedSPolyID] = in_polyDebugLevel;
	relationshipDebugFlags[in_trackedSPolyID].handleSpecificTrackedSPolyDebugOption(in_debugOption);
}

void CoplanarRelationshipTracker::insertDOForSpecificTrackedSPolySTriangle(int in_trackedSPolyID, int in_trackedSPolySTriangleID, DebugOption in_debugOption)
{
	relationshipDebugFlags[in_trackedSPolyID].handleSpecificTrackedSPolySTriangleDebugOption(in_trackedSPolySTriangleID, in_debugOption);
}

void CoplanarRelationshipTracker::insertDOForSpecificTrackedCutterCuttingTriangle(int in_trackedSPolyID, int in_trackedSPolyCuttingTriangleID, DebugOption in_debugOption)
{
	relationshipDebugFlags[in_trackedSPolyID].handleSpecificTrackedSPolyCutterCuttingTriangleDebugOption(in_trackedSPolyCuttingTriangleID, in_debugOption);
}

PolyDebugLevel CoplanarRelationshipTracker::retrieveDebugLevelOptionIfExistent(int in_trackedSPolyID)
{
	PolyDebugLevel returnLevel = PolyDebugLevel::NONE;

	return returnLevel;
}

void CoplanarRelationshipTracker::insertRelationship(int in_trackedSPolyID, SPoly* in_trackedSPolyRef, int in_relatedSPolyID, SPoly* in_relatedSPolyRef)
{
	// it doesn't exist, so we'll have to insert a fresh relationship;
	// we must set the tracked poly data (via setTrackedPolyData),
	// and the logger debug level for the relationship (via setLoggerDebugLevel)
	if (auto checkIfRelationshipForTrackedPolyExists = relationshipContainer.find(in_trackedSPolyID); checkIfRelationshipForTrackedPolyExists == relationshipContainer.end())	
	{
		relationshipContainer[in_trackedSPolyID].setTrackedPolyData(in_trackedSPolyID, *in_trackedSPolyRef);
		relationshipContainer[in_trackedSPolyID].setCoplanarRelationshipBoxType(coplanarComparisonBoxType);
		relationshipContainer[in_trackedSPolyID].setLoggerDebugLevel(relationshipTrackerDebugLevel);
	}
	relationshipContainer[in_trackedSPolyID].insertRelationship(in_relatedSPolyID, *in_relatedSPolyRef);
}

void CoplanarRelationshipTracker::applyCoplanarRelationshipDebugFlagsIfFound(int in_trackedSPolyID)
{
	if (auto checkIfRelationshipDebugFlagsForTrackedPolyExist = relationshipDebugFlags.find(in_trackedSPolyID); checkIfRelationshipDebugFlagsForTrackedPolyExist != relationshipDebugFlags.end())
	{
		relationshipContainer[in_trackedSPolyID].applyCoplanarRelationshipDebugOptions(&relationshipDebugFlags[in_trackedSPolyID]);
	}
}

void CoplanarRelationshipTracker::printRelationshipData()
{
	auto relationshipContainerBegin = relationshipContainer.begin();
	auto relationshipContainerEnd = relationshipContainer.end();
	for (; relationshipContainerBegin != relationshipContainerEnd; relationshipContainerBegin++)
	{
		std::cout << "(CoplanarRelationshipTracker) >>> Printing out coplanar relationships of related SPolys, for the tracked SPoly: " << relationshipContainerBegin->first << std::endl;
		auto relatedSPolysBegin = relationshipContainerBegin->second.relationshipMap.refMap.begin();
		auto relatedSPolysEnd = relationshipContainerBegin->second.relationshipMap.refMap.end();
		for (; relatedSPolysBegin != relatedSPolysEnd; relatedSPolysBegin++)
		{
			std::cout << "Related SPoly ID: " << relatedSPolysBegin->first << std::endl;
		}
	}
}

void CoplanarRelationshipTracker::printRemovableSPolys()
{
	std::cout << "(CoplanarRelationshipTracker): Printing removable SPolys..." << std::endl;
	auto removablesBegin = removableSPolys.intSet.begin();
	auto removablesEnd = removableSPolys.intSet.end();
	for (; removablesBegin != removablesEnd; removablesBegin++)
	{
		std::cout << *removablesBegin << std::endl;
	}
}

void CoplanarRelationshipTracker::setCoplanarMassZoneBoxType(MassZoneBoxType in_massZoneBoxType)
{
	coplanarComparisonBoxType = in_massZoneBoxType;
}