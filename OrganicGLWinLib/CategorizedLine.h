#pragma once

#ifndef CATEGORIZEDLINE_H
#define CATEGORIZEDLINE_H

#include "IntersectionLine.h"
#include "IntersectionType.h"
#include "IRPointType.h"
#include "CyclingDirection.h"
#include "QuatRotationManager.h"
#include "PointTranslationCheck.h"
#include "QuatRotationManager.h"
//#include <mutex>
#include "CategorizedLineOptionals.h"
#include "PolyLogger.h"
#include "PolyDebugLevel.h"
#include "QMVoidFindCyclingDirection.h"
#include <sstream>
#include <iomanip>


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
		containsExtraData = categorizedLine_b.containsExtraData;
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
		containsExtraData = categorizedLine_b.containsExtraData;
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
	bool containsExtraData = false;					// needs to be set, when the CategorizedLine IntersectionType is A_SLICE_DOUBLE_INTERCEPTS_POINT_PRECISE, or A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE
	std::unique_ptr<CategorizedLineOptionals> extraData;

	void convertLineToPartialBound(IntersectionLine in_lineA, IntersectionLine in_lineB, glm::vec3 in_newPointForLineA);
	void convertLineToPartialBound(IntersectionLine in_lineA, IntersectionLine in_lineB);
	void convertLineToNonbound(IntersectionLine in_engulfedLine);
	void convertLinesToNonbound(IntersectionLine in_lineA, IntersectionLine in_lineB);
	void convertLinesToInterceptsPointPrecise(IntersectionLine in_lineA, IntersectionLine in_lineB);

	bool determineCyclingDirection(std::map<int, SPolyBorderLines> in_borderLineArrayRef, PolyDebugLevel in_polyDebugLevel);
	void generateCyclingDirectionForInterceptPointPrecise(SPolyBorderLines in_borderLineA, 
													      int in_borderLineAID, 
														  SPolyBorderLines in_borderLineB, 
		                                                  int in_borderLineBID, 
		                                                  glm::vec3 in_categorizedLineNormal,
														  PolyDebugLevel in_polyDebugLevel);
	void generateCyclingDirectionForASliceSingleInterceptPointPrecise(SPolyBorderLines in_borderLineA,
		int in_borderLineAID,
		SPolyBorderLines in_borderLineB,
		int in_borderLineBID,
		glm::vec3 in_categorizedLineNormal,
		PolyDebugLevel in_polyDebugLevel);

	void roundLineEndpoints();
	std::string writeLineToString();

	void testFunction();
	void createCategorizedLineOptionals();
	void insertOptionalDataForPoint(IRPointType in_irPointType, std::vector<FusedPointSubData> in_fusedPointSubDataVector);
	std::string getIntersectionTypeString();

	void convertLineToSlice(IntersectionLine in_slicingLine);
	IRPointType checkIfPointIsInLine(glm::vec3 in_point);
	IRPointType checkIfPointIsNearbyPointInLine(glm::vec3 in_point, float in_searchBoxHalfDiameter);
	void setEmptyNormal(glm::vec3 in_emptyNormal);
};

#endif
