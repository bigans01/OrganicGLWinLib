#pragma once

#ifndef INTERSECTIONLINEGROUP_H
#define INTERSECTIONLINEGROUP_H

#include <map>
#include "IntersectionLine.h"
#include <iostream>

class IntersectionLineGroup
{
public:
	int numberOfIntersectionLines = 0;			// always starts at 0
	std::map<int, IntersectionLine> lineMap;

	void addIntersectionLine(IntersectionLine in_intersectionLine);
	IntersectionLine mergeLines();
	bool performInvalidCheck();					// may not be worth using; flag for removal.
	int findNumberOfBorderLineIntercepts();
	void reset();								// reset the line group, so it has appropriate values in next iteration
	IntersectionLine* findSingleBorderLine();
	IntersectionLine* findSingleNonBorderLine();
};

#endif