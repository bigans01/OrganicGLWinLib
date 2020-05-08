#pragma once

#ifndef INTERSECTIONRESULT_H
#define INTERSECTIONRESULT_H

#include <glm/glm.hpp>

class IntersectionResult
{
public:
	short wasIntersectFound = 0;	// was there an actual intersect?
	short wasIntersectOnBorderLine = 0;		// was this intersect caused by a border line?
	int borderLineID = 0;			// if the intersect was on a border line, what was the border line's ID?
	glm::vec3 intersectedPoint;		// what was the point that was intersected?

	void setResult(int in_result);
	int wasResultSet = 0;			// indicates if a result was already set
};

#endif
