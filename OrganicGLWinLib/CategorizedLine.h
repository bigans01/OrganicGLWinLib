#pragma once

#ifndef CATEGORIZEDLINE_H
#define CATEGORIZEDLINE_H

#include "IntersectionLine.h"
#include "IntersectionType.h"
#include "IRPointType.h"
#include <iostream>
#include <glm/glm.hpp>
#include "CyclingDirection.h"
//#include "SPolyBorderLines.h"
#include "QuatRotationManager.h"
#include "PointTranslationCheck.h"
#include "QuatRotationManager.h"


class SPolyBorderLines;
class CategorizedLine
{
public:
	IntersectionLine line;
	IntersectionType type = IntersectionType::NONE; // always starts as "NONE"
	glm::vec3 emptyNormal;
	CyclingDirection direction = CyclingDirection::NOVAL;	// default value is NOVAL; may be set otherwise.
	int cleaveSequenceID = 0;						// represents the ID that this categorized line uses in the CleaveSequence it goes to 
													//	(that is, if it even gets to a CleaveSequence; it may be dropped/deleted/discarded before getting there.).

	void convertLineToPartialBound(IntersectionLine in_lineA, IntersectionLine in_lineB, glm::vec3 in_newPointForLineA);
	void convertLineToPartialBound(IntersectionLine in_lineA, IntersectionLine in_lineB);
	void convertLineToNonbound(IntersectionLine in_engulfedLine);
	void convertLinesToNonbound(IntersectionLine in_lineA, IntersectionLine in_lineB);
	void convertLinesToInterceptsPointPrecise(IntersectionLine in_lineA, IntersectionLine in_lineB);

	void determineCyclingDirection(SPolyBorderLines(&in_borderLineArrayRef)[8]);
	void generateCyclingDirectionForInterceptPointPrecise(SPolyBorderLines in_borderLineA, int in_borderLineAID, SPolyBorderLines in_borderLineB, int in_borderLineBID, glm::vec3 in_categorizedLineNormal);
	
	void convertLineToSlice(IntersectionLine in_slicingLine);
	IRPointType checkIfPointIsInLine(glm::vec3 in_point);
	void setEmptyNormal(glm::vec3 in_emptyNormal);
};

#endif
