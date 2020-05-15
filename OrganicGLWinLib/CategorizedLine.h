#pragma once

#ifndef CATEGORIZEDLINE_H
#define CATEGORIZEDLINE_H

#include "IntersectionLine.h"
#include "IntersectionType.h"
#include "IRPointType.h"
#include <iostream>
#include <glm/glm.hpp>

class CategorizedLine
{
public:
	IntersectionLine line;
	IntersectionType type = IntersectionType::NONE; // always starts as "NONE"
	glm::vec3 emptyNormal;

	void convertLineToPartialBound(IntersectionLine in_lineA, IntersectionLine in_lineB, glm::vec3 in_newPointForLineA);
	void convertLineToPartialBound(IntersectionLine in_lineA, IntersectionLine in_lineB);
	void convertLineToNonbound(IntersectionLine in_engulfedLine);
	void convertLinesToNonbound(IntersectionLine in_lineA, IntersectionLine in_lineB);
	void convertLineToSlice(IntersectionLine in_slicingLine);
	IRPointType checkIfPointIsInLine(glm::vec3 in_point);
	void setEmptyNormal(glm::vec3 in_emptyNormal);
};

#endif