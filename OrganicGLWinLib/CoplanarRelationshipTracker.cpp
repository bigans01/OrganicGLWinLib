#include "stdafx.h"
#include "CoplanarRelationshipTracker.h"

void CoplanarRelationshipTracker::insertCoplanarRelationship(int in_trackedSPolyID, SPoly* in_trackedSPolyRef, int in_relatedSPolyID, SPoly* in_relatedSPolyRef)
{
	
	auto checkIfRelationshipForTrackedPolyExists = relationshipContainer.find(in_trackedSPolyID);
	if (checkIfRelationshipForTrackedPolyExists == relationshipContainer.end())	// it doesn't exist, so we'll have to insert a fresh relationship.
	{
		relationshipContainer[in_trackedSPolyID].setTrackedPolyData(in_trackedSPolyID, in_trackedSPolyRef);
	}
	relationshipContainer[in_trackedSPolyID].insertRelationship(in_relatedSPolyID, in_relatedSPolyRef);

	std::cout << "!! Verifying pointers are OK: " << std::endl;
	std::cout << "-> tracked SPoly, number of border lines: " << relationshipContainer[in_trackedSPolyID].trackedSPolyRef->numberOfBorderLines << std::endl;
	std::cout << "-> related SPoly, number of border lines: " << relationshipContainer[in_trackedSPolyID].relationshipMap.refMap[in_relatedSPolyID]->numberOfBorderLines << std::endl;
	
}
void CoplanarRelationshipTracker::buildCoplanarCategorizedLines()
{
	
	auto relationshipsBegin = relationshipContainer.begin();
	auto relationshipsEnd = relationshipContainer.end();
	for (; relationshipsBegin != relationshipsEnd; relationshipsBegin++)
	{
		std::cout << "-> Found CoplanarRelationship, for tracked SPoly -> " << relationshipsBegin->first << std::endl;
		relationshipsBegin->second.rotateToXYPlaneAndCompare();
	}
	
}