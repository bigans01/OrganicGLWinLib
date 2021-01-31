#pragma once

#ifndef POINTTOMASSRELATIONSHIP_H
#define POINTTOMASSRELATIONSHIP_H

#include <glm/glm.hpp>
#include "PointToMassRelationshipType.h"

class PointToMassRelationship
{
	public:
		PointToMassRelationship() {};
		PointToMassRelationship(glm::vec3 in_relationshipPoint)
			: point(in_relationshipPoint) {};
	private:
		friend class PointToMassRelationshipMap;
		glm::vec3 point;
		PointToMassRelationshipType relationshipStatus = PointToMassRelationshipType::NOVAL;
};

#endif
