#pragma once

#ifndef EMPTYNORMALFINDER_H
#define EMPTYNORMALFINDER_H

#include "QuatRotationRecord.h"
#include "QuatRotationPoints.h"
#include "QuatRotationManager.h"
#include "PointTranslationCheck.h"

/*
* 
* Description: This class takes in 4 values: 3 points of a triangle, plus a mass reference point. It finds the minimum X/Y/Z values of the triangle,
* to form a translation value, which is then used to translate all 4 values. Then, the centroid of the translated triangle is found. The value of 
* the centroid minus the translated MRP is then normalized via glm::normalize, to produce a the empty normal that faces the direction of "empty space".

No translation of points is required prior to using the constructors; translatiing the points prior to this call should have no affect on the outcome either.
*/

class EmptyNormalFinder
{
	public:
		EmptyNormalFinder(DoublePoint in_point0, DoublePoint in_point1, DoublePoint in_point2, DoublePoint in_massReferencePoint);
		EmptyNormalFinder(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2, glm::vec3 in_massReferencePoint);
		glm::vec3 getFinderCalculatedNormal();
	private:
		glm::vec3 calculatedNormal;		// the resulting calculation from the constructors gets stored here, and then retrieved by the call to
										// getFinderCalculatedNormal.


};

#endif
