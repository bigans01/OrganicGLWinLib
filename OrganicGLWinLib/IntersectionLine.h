#pragma once

#ifndef INTERSECTIONLINE_H
#define INTERSECTIONLINE_H

#include <glm/glm.hpp>
#include "IntersectionResult.h"

class IntersectionLine
{
public:
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
};

#endif

