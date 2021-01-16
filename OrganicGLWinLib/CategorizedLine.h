#pragma once

#ifndef CATEGORIZEDLINE_H
#define CATEGORIZEDLINE_H

#include "IntersectionLine.h"
#include "IntersectionType.h"
#include "IRPointType.h"
#include <iostream>
#include <glm/glm.hpp>
#include "CyclingDirection.h"
#include "QuatRotationManager.h"
#include "PointTranslationCheck.h"
#include "QuatRotationManager.h"
#include <map>
#include <mutex>
#include "CategorizedLineOptionals.h"


class SPolyBorderLines;
class CategorizedLine
{
public:
	CategorizedLine() {};
	CategorizedLine(const CategorizedLine& categorizedLine_b)
	{
		line = categorizedLine_b.line;
		type = categorizedLine_b.type;
		emptyNormal = categorizedLine_b.emptyNormal;
		direction = categorizedLine_b.direction;
		cleaveSequenceID = categorizedLine_b.cleaveSequenceID;
		parentPoly = categorizedLine_b.parentPoly;
		if (categorizedLine_b.containsExtraData == true)
		{
			extraData.reset(new CategorizedLineOptionals);
			*extraData = *categorizedLine_b.extraData;
		}
	}
	
	CategorizedLine& operator=(const CategorizedLine& categorizedLine_b)
	{
		line = categorizedLine_b.line;
		type = categorizedLine_b.type;
		emptyNormal = categorizedLine_b.emptyNormal;
		direction = categorizedLine_b.direction;
		cleaveSequenceID = categorizedLine_b.cleaveSequenceID;
		parentPoly = categorizedLine_b.parentPoly;
		if (categorizedLine_b.containsExtraData == true)
		{
			extraData.reset(new CategorizedLineOptionals);
			*extraData = *categorizedLine_b.extraData;
		}
		return *this;
	}
	

	IntersectionLine line;
	IntersectionType type = IntersectionType::NONE; // always starts as "NONE"
	glm::vec3 emptyNormal;
	CyclingDirection direction = CyclingDirection::NOVAL;	// default value is NOVAL; may be set otherwise.
	int cleaveSequenceID = 0;						// represents the ID that this categorized line uses in the CleaveSequence it goes to 
	int parentPoly = 0;								// the index ID of the SPoly in the SPolySet that spawned this line.
													//	(that is, if it even gets to a CleaveSequence; it may be dropped/deleted/discarded before getting there.).
	bool containsExtraData = false;					// needs to be set, when the CategorizedLine IntersectionType is A_SLICE_DOUBLE_INTERCEPTS_POINT_PRECISE.
	std::unique_ptr<CategorizedLineOptionals> extraData;

	void convertLineToPartialBound(IntersectionLine in_lineA, IntersectionLine in_lineB, glm::vec3 in_newPointForLineA);
	void convertLineToPartialBound(IntersectionLine in_lineA, IntersectionLine in_lineB);
	void convertLineToNonbound(IntersectionLine in_engulfedLine);
	void convertLinesToNonbound(IntersectionLine in_lineA, IntersectionLine in_lineB);
	void convertLinesToInterceptsPointPrecise(IntersectionLine in_lineA, IntersectionLine in_lineB);

	void determineCyclingDirection(std::map<int, SPolyBorderLines> in_borderLineArrayRef);
	void generateCyclingDirectionForInterceptPointPrecise(SPolyBorderLines in_borderLineA, int in_borderLineAID, SPolyBorderLines in_borderLineB, int in_borderLineBID, glm::vec3 in_categorizedLineNormal);

	void testFunction();

	void convertLineToSlice(IntersectionLine in_slicingLine);
	IRPointType checkIfPointIsInLine(glm::vec3 in_point);
	void setEmptyNormal(glm::vec3 in_emptyNormal);
};

#endif
