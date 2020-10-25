#include "stdafx.h"
#include "CategorizedLine.h"


void CategorizedLine::convertLineToPartialBound(IntersectionLine in_lineA, IntersectionLine in_lineB, glm::vec3 in_newPointForLineA)
{
	type = IntersectionType::PARTIAL_BOUND;
	line.numberOfBorderLines = 1;		// PARTIAL_BOUND always has one border line, and a non-bound point.
	line.isPointAOnBorder = 1;											// point A should always be the point on a border in a PARTIAL_BOUND type (need to verify, 8/2/2019)
	line.pointA = in_lineA.pointA;										// set point A
	line.pointB = in_newPointForLineA;									// set point B, set in the previous function
	line.intersectedSecondaryID = in_lineA.intersectedSecondaryID;		// set the secondary ID, which is the value of the polygon that caused the intersection
}

void CategorizedLine::convertLineToPartialBound(IntersectionLine in_lineA, IntersectionLine in_lineB)
{
	type = IntersectionType::PARTIAL_BOUND;		// TWIN always has exactly one point on a border line, and a non-bound point.
	line.numberOfBorderLines = 1;
	line.isPointAOnBorder = 1;
	line.pointA = in_lineA.pointA;
	line.pointB = in_lineB.pointA;
	line.intersectedSecondaryID = in_lineA.intersectedSecondaryID;
}

void CategorizedLine::convertLineToNonbound(IntersectionLine in_engulfedLine)
{
	type = IntersectionType::NON_BOUND;
	line.numberOfBorderLines = 0;		// NON_BOUND will never have any border lines...
	line.pointA = in_engulfedLine.pointA;		// ...but it's points are equal to the "engulfed" line
	line.pointB = in_engulfedLine.pointB;
	line.intersectedSecondaryID = in_engulfedLine.intersectedSecondaryID;
}

void CategorizedLine::convertLinesToNonbound(IntersectionLine in_lineA, IntersectionLine in_lineB)
{
	type = IntersectionType::NON_BOUND;
	line.numberOfBorderLines = 0;
	line.pointA = in_lineA.pointA;
	line.pointB = in_lineB.pointA;
	line.intersectedSecondaryID = in_lineA.intersectedSecondaryID;
}

void CategorizedLine::convertLinesToInterceptsPointPrecise(IntersectionLine in_lineA, IntersectionLine in_lineB)
{
	// if in_lineA is the invalid one, use the points in  in_lineB.
	if (in_lineA.lineValidity == IntersectionLineValidity::INVALID)
	{
		std::cout << "::::: Line categorization, INTERCEPT_POINT_PRECISE, lineA " << std::endl;

		type = IntersectionType::INTERCEPTS_POINT_PRECISE;

		// we don't know which border lines we will use yet, so set the value to two, and copy the values from the invalid line.
		line.pointA = in_lineB.pointA;
		line.pointB = in_lineB.pointB;
		line.numberOfBorderLines = 2;
		line.isPointAOnBorder = 1;
		line.pointABorder = in_lineA.isPointAOnBorder;
		line.isPointBOnBorder = 1;
		line.pointBBorder = in_lineA.isPointAOnBorder;
		
		
	}
	else if (in_lineB.lineValidity == IntersectionLineValidity::INVALID)
	{
		std::cout << "::::: Line categorization, INTERCEPT_POINT_PRECISE, lineB " << std::endl;

		type = IntersectionType::INTERCEPTS_POINT_PRECISE;

		line.pointA = in_lineA.pointA;
		line.pointB = in_lineA.pointB;
		line.numberOfBorderLines = 2;
		line.isPointAOnBorder = 1;
		line.pointABorder = in_lineA.isPointAOnBorder;
		line.isPointBOnBorder = 1;
		line.pointBBorder = in_lineA.isPointAOnBorder;
	}
	// do for in_lineB? maybe.
}


void CategorizedLine::convertLineToSlice(IntersectionLine in_slicingLine)
{
	type = IntersectionType::A_SLICE;
	line.numberOfBorderLines = 2;
	std::cout << "Converting line to slice..." << std::endl;
	line.isPointAOnBorder = 1;
	line.pointA = in_slicingLine.pointA;
	line.pointABorder = in_slicingLine.pointABorder;
	line.isPointBOnBorder = 1;
	line.pointB = in_slicingLine.pointB;
	line.pointBBorder = in_slicingLine.pointBBorder;
	line.intersectedSecondaryID = in_slicingLine.intersectedSecondaryID;
}

IRPointType CategorizedLine::checkIfPointIsInLine(glm::vec3 in_point)
{
	IRPointType returnType = IRPointType::NEITHER;	// default value
	if
		(
			line.pointA.x == in_point.x
			&&
			line.pointA.y == in_point.y
			&&
			line.pointA.z == in_point.z
			)
	{
		returnType = IRPointType::POINT_A;
	}

	else if
		(
			line.pointB.x == in_point.x
			&&
			line.pointB.y == in_point.y
			&&
			line.pointB.z == in_point.z
			)
	{
		returnType = IRPointType::POINT_B;
	}
	return returnType;
}

void CategorizedLine::setEmptyNormal(glm::vec3 in_emptyNormal)
{
	emptyNormal = in_emptyNormal;
}