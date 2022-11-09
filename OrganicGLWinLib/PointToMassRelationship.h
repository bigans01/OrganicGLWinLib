#pragma once

#ifndef POINTTOMASSRELATIONSHIP_H
#define POINTTOMASSRELATIONSHIP_H

#include "PointToMassRelationshipType.h"

class PointToMassRelationship
{
	public:
		PointToMassRelationship() {};
		PointToMassRelationship(glm::vec3 in_relationshipPoint)
			: point(in_relationshipPoint) {};
	private:
		friend class PointToMassRelationshipMap;
		friend class MassZonePointClipper;
		glm::vec3 point;
		PointToMassRelationshipType relationshipStatus = PointToMassRelationshipType::NOVAL;
};

#endif
