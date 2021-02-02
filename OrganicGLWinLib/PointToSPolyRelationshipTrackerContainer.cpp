#include "stdafx.h"
#include "PointToSPolyRelationshipTrackerContainer.h"

void PointToSPolyRelationshipTrackerContainer::insertRelationshipTrackerData(glm::vec3 in_point, int in_sPolyID, int in_sTriangleID, STriangle* in_sTriangleRef)
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
		relationshipTrackerContainer[existingPointKeyValue].insertPointToSPolyRelationship(in_sPolyID, in_sTriangleID, in_sTriangleRef);
	}
	else                                    // insert a new record
	{
		int containerSize = int(relationshipTrackerContainer.size());
		relationshipTrackerContainer[containerSize].point = in_point;			// make sure to set this when inserting a new relationship tracker!
		relationshipTrackerContainer[containerSize].insertPointToSPolyRelationship(in_sPolyID, in_sTriangleID, in_sTriangleRef);
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