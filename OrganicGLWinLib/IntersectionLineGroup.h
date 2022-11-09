#pragma once

#ifndef INTERSECTIONLINEGROUP_H
#define INTERSECTIONLINEGROUP_H

#include "IntersectionLine.h"
#include "PolyLogger.h"
#include "PolyDebugLevel.h"
#include "FusedIntersectionLine.h"
#include "IntersectionResult.h"
#include "SPoly.h"
#include "FusionCandidateOrigin.h"

class IntersectionLineGroup
{
public:
	IntersectionLineGroup(PolyDebugLevel in_polyDebugLevel);
	int numberOfIntersectionLines = 0;			// always starts at 0
	std::map<int, IntersectionLine> lineMap;
	void setFusionAnalyzerSPolyRef(SPoly* in_sPolyRef);
	void addIntersectionLine(IntersectionLine in_intersectionLine);
	void runFusionAnalysisAndDetermineClassifications();
	int findNumberOfBorderLineIntercepts();
	void reset();								// reset the line group, so it has appropriate values in next iteration
	IntersectionLine* findSingleBorderLine();
	IntersectionLine* findSingleNonBorderLine();
	void insertFusionCandidateIntoAnalyzer(FusionCandidateOrigin in_fusionCandidateOrigin, int in_lineIndex, FusionCandidate in_fusionCandidate);
	FusedIntersectionLine returnLine;
private:
	PolyLogger lineGroupLogger;
	//FusedIntersectionLine returnLine;
};

#endif