#pragma once

#ifndef INTERSECTIONLINE_H
#define INTERSECTIONLINE_H

#include <glm/glm.hpp>
#include "IntersectionResult.h"
#include "BorderLineIDPair.h"
#include "BorderLinePointPair.h"
#include <iostream>
#include "IntersectionLineValidity.h"
#include "CyclingDirection.h"

class IntersectionLine
{
public:

	IntersectionLineValidity lineValidity = IntersectionLineValidity::VALID;	// the default value is VALID, but becomes INVALID if both points of the line are the same.

	int numberOfBorderLines = 0;	// the number of border lines this line "touches"
	int numberOfPoints = 0;
	int lineGroupID = 0;			// the group ID this line belongs to
	int intersectedSecondaryID = 0;	// what is the ID of the secondary that the interception occurred on?

	glm::vec3 pointA;
	short isPointAOnBorder = 0;
	short pointABorder = 0;			

	glm::vec3 pointB;
	short isPointBOnBorder = 0;
	short pointBBorder = 0;			

	void addIntersectionResult(IntersectionResult in_intersectResult);
	void swapBorderToA();
	void swapBorderToB();
	void swapToA();

	int getBorderLineIDFromSingularBorderLineCount();
	BorderLineIDPair getBorderLineIDPair();
	BorderLinePointPair getBorderLinePointPair();
	

	glm::vec3 getBorderPointFromSingularBorderLineCount();
	glm::vec3 getNonBorderPointFromSingularBorderLineCount();

	glm::vec3 fetchNextPointBasedOnCyclingDirection(CyclingDirection in_direction);
};

#endif

