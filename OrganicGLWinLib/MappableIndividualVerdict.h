#pragma once

#ifndef MAPPABLEINDIVIDUALVERDICT_H
#define MAPPABLEINDIVIDUALVERDICT_H

#include "IndividualVerdict.h"
#include "PointToMassRelationshipType.h"

class MappableIndividualVerdict
{
	public:
		MappableIndividualVerdict() {};
		MappableIndividualVerdict(glm::vec3 in_point, IndividualVerdict in_individualVerdict) :
			point(in_point),
			shouldBeClipped(in_individualVerdict.shouldBeClipped),
			relationshipTypeMap(in_individualVerdict.relationshipTypeMap)
		{};

		glm::vec3 point;
		bool shouldBeClipped = false;
		std::map<int, PointToMassRelationshipType> relationshipTypeMap;

		bool equals(glm::vec3 in_point)
		{
			bool doesPointEqualInput = false;
			if (in_point == point)
			{
				doesPointEqualInput =  true;
			}
			return doesPointEqualInput;
		}
};

#endif
