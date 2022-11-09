#pragma once

#ifndef INDIVIDUALVERDICT_H
#define INDIVIDUALVERDICT_H

#include "PointToMassRelationshipType.h"

class IndividualVerdict
{
	public:
		IndividualVerdict() {};
		IndividualVerdict(bool in_shouldBeClipped, std::map<int, PointToMassRelationshipType> in_relationshipTypeMap) :
			shouldBeClipped(in_shouldBeClipped),
			relationshipTypeMap(in_relationshipTypeMap)
		{};
		bool shouldBeClipped = false;
		std::map<int, PointToMassRelationshipType> relationshipTypeMap;
};

#endif