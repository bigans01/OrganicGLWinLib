#pragma once

#ifndef INTERSECTIONLINEGROUP_H
#define INTERSECTIONLINEGROUP_H

#include <map>
#include "IntersectionLine.h"
#include <iostream>
#include "PolyLogger.h"
#include "PolyDebugLevel.h"
#include "FusedIntersectionLine.h"
#include "IntersectionResult.h"
#include "SPoly.h"

class IntersectionLineGroup
{
public:
	IntersectionLineGroup(PolyDebugLevel in_polyDebugLevel);
	int numberOfIntersectionLines = 0;			// always starts at 0
	std::map<int, IntersectionLine> lineMap;
	void setFusionAnalyzerSPolyRef(SPoly* in_sPolyRef);
	void addIntersectionLine(IntersectionLine in_intersectionLine);
	FusedIntersectionLine mergeLines();
	bool performInvalidCheck();					// may not be worth using; flag for removal.
	int findNumberOfBorderLineIntercepts();
	void reset();								// reset the line group, so it has appropriate values in next iteration
	IntersectionLine* findSingleBorderLine();
	IntersectionLine* findSingleNonBorderLine();
	void insertFusionCandidateIntoAnalyzer(FusionCandidate in_fusionCandidate, IntersectionResult in_intersectResult);
private:
	PolyLogger lineGroupLogger;
	FusedIntersectionLine returnLine;
};

#endif