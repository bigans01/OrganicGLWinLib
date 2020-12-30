#pragma once

#ifndef CATEGORIZEDLINEGROUPLOCATION_H
#define CATEGORIZEDLINEGROUPLOCATION_H

#include "IntersectionType.h"

class CategorizedLineGroupLocation
{
	public:
		CategorizedLineGroupLocation() {};
		CategorizedLineGroupLocation(int in_locationParentSPolyID, IntersectionType in_locationIntersectionType, int in_locationPoolIndex) :
			locationParentSPolyID(in_locationParentSPolyID),
			locationIntersectionType(in_locationIntersectionType),
			locationPoolIndex(in_locationPoolIndex)
		{};
		int locationParentSPolyID = 0;
		IntersectionType locationIntersectionType = IntersectionType::NONE;
		int locationPoolIndex = 0;
};

#endif
