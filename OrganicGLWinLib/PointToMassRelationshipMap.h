#pragma once

#ifndef POINTTOMASSRELATIONSHIPMAP_H
#define POINTTOMASSRELATIONSHIPMAP_H

#include "PointToMassRelationship.h"

class PointToMassRelationshipMap
{
	public:
		void insertNewPoint(glm::vec3 in_relationshipPoint)
		{
			PointToMassRelationship newRelationship(in_relationshipPoint);
			int relationshipMapSize = int(relationshipMap.size());
			relationshipMap[relationshipMapSize] = newRelationship;
		}
	private:
		friend class MassZonePointClipper;
		std::map<int, PointToMassRelationship> relationshipMap;
};

#endif