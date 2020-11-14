#pragma once

#ifndef COPLANARRELATIONSHIPTRACKER_H
#define COPLANARRELATIONSHIPTRACKER_H

#include <map>
#include "CoplanarRelationships.h"
#include "SPoly.h"


class CoplanarRelationshipTracker
{

public:
	std::map<int, CoplanarRelationships> relationshipContainer;
	void insertCoplanarRelationship(int in_trackedSPolyID, SPoly* in_trackedSPolyRef, int in_relatedSPolyID, SPoly* in_relatedSPolyRef)
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
	void buildCategorizedLinesForCoplanarRelationship(int in_trackedSPolyID)
	{

	}
};

#endif
