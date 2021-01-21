#include "stdafx.h"
#include "HostLineReactor.h"

void HostLineReactor::runAnalysis()
{
	std::cout << "(HostLineReactor): running analysis..." << std::endl;

	// search for the following, in the appropriate order:
	// Search 1: PRECISE_NONBORDERLINE
	
	// Search 2: PRECISE_BORDERLINE
	if (fusedPointMetaRef->searchForSummary(FusedPointSummary::PRECISE_BORDERLINE).wasFound == true)
	{
		glm::vec3 precisePoint = fusedPointMetaRef->searchForSummary(FusedPointSummary::PRECISE_BORDERLINE).foundPoint;
		SummaryScanResult otherSummary = fusedPointMetaRef->searchForOtherSummary(precisePoint);
		if (otherSummary.pointSummary == FusedPointSummary::TYPICAL_NONBORDERLINE)	// we have a PRECISE_BORDERLINE point, followed by the other point being nonbound; 
																				// this must be a PARTIAL_BOUND.
		{
			std::cout << "(HostLineReactor) Search 2-> TYPICAL_NONBORDERLINE found; processing for INTERCEPTS_POINT_PRECISE." << std::endl;
			buildInterceptsPointPrecise(precisePoint, otherSummary.foundPoint);
		}
		else if (otherSummary.pointSummary == FusedPointSummary::TYPICAL_BORDERLINE)
		{
			std::cout << "(HostLineReactor) Search 2-> TYPICAL_BORDERLINE found; processing for A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE." << std::endl;
		}
	}

	else if (fusedPointMetaRef->searchForSummary(FusedPointSummary::PRECISE_MIXED).wasFound == true)
	{

	}

	else if (fusedPointMetaRef->searchForSummary(FusedPointSummary::PRECISE_NONBORDERLINE).wasFound == true)
	{

	}

	// Search 3: TYPICAL_BORDERLINE
	else if (fusedPointMetaRef->searchForSummary(FusedPointSummary::TYPICAL_BORDERLINE).wasFound == true)
	{
		glm::vec3 borderPoint = fusedPointMetaRef->searchForSummary(FusedPointSummary::TYPICAL_BORDERLINE).foundPoint;
		SummaryScanResult otherSummary = fusedPointMetaRef->searchForOtherSummary(borderPoint);
		if (otherSummary.pointSummary == FusedPointSummary::TYPICAL_BORDERLINE) // the first point is a TYPICAL_BORDERLINE, and so is this one; it's a SLICE.
		{
			std::cout << "(HostLineReactor) Search 3 -> TYPICAL_BORDERLINE found; processing for A_SLICE. " << std::endl;
		}
		else if (otherSummary.pointSummary == FusedPointSummary::TYPICAL_NONBORDERLINE) // the first point is a TYPICAL_BORDERLINE, but this one is TYPICAL_NONBORDERLINE; it's a PARTIAL_BOUND.
		{
			std::cout << "(HostLineReactor) Search 3 -> TYPICAL_NONBORDERLINE found; processing for PARTIAL_BOUND. " << std::endl;
		}
	}

	// Search 4: TYPICAL_NONBORDERLINE
	else if (fusedPointMetaRef->searchForSummary(FusedPointSummary::TYPICAL_NONBORDERLINE).wasFound == true)
	{

	}
}

void HostLineReactor::buildInterceptsPointPrecise(glm::vec3 in_buildStartPoint, glm::vec3 in_otherPoint)
{
	std::cout << "(HostLineReactor): building INTERCEPTS_POINT_PRECISE..." << std::endl;
	// remember, always set point A as the "one on the border"
	std::vector<FusedPointSubData>* subDataRef = hostFusionAnalysisRef->fusedPoints.fetchSubDataVectorForPoint(in_buildStartPoint);
	FusedPointSubData subDataArray[2];
	auto subDataRefBegin = subDataRef->begin();
	auto subDataRefEnd = subDataRef->end();
	int beginIndex = 0;
	for (; subDataRefBegin != subDataRefEnd; subDataRefBegin++)
	{
		subDataArray[beginIndex++] = *subDataRefBegin;
	}

	// testing, print out the contents of the sub data
	for (int x = 0; x < 2; x++)
	{
		std::cout << "Sub data at index " << x;
		std::cout << ": isBorderLine: " << subDataArray[x].isBorderLine;
		std::cout << ": borderLineID: " << subDataArray[x].borderLineValue << std::endl;
	}

	resultantLine.type = IntersectionType::INTERCEPTS_POINT_PRECISE;
	resultantLine.line.pointA = in_buildStartPoint;							// point A is the precise point
	resultantLine.line.pointB = in_otherPoint;								// point B is the other point
	resultantLine.line.pointABorder = subDataArray[0].borderLineValue;		// load border line values
	resultantLine.line.pointBBorder = subDataArray[1].borderLineValue;		// "" 
	resultantLine.line.numberOfBorderLines = 1;

	std::cout << "(HostLineReactor): finished producing INTERCEPT_POINTS_PRECISE, stats are: " << std::endl;
	std::cout << "(HostLineReactor): point A: " << resultantLine.line.pointA.x << ", " << resultantLine.line.pointA.y << ", " << resultantLine.line.pointA.z << std::endl;
	std::cout << "(HostLineReactor): point B: " << resultantLine.line.pointB.x << ", " << resultantLine.line.pointB.y << ", " << resultantLine.line.pointB.z << std::endl;
	std::cout << "(HostLineReactor): pointABorder: " << resultantLine.line.pointABorder << std::endl;
	std::cout << "(HostLineReactor): pointBBorder: " << resultantLine.line.pointBBorder << std::endl;
}