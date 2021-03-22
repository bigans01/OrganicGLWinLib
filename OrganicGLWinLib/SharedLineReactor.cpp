#include "stdafx.h"
#include "SharedLineReactor.h"

void SharedLineReactor::runAnalysis()
{
	//std::cout << "(SharedLineReactor): running analysis..." << std::endl;
	reactorBaseLogger.log("(SharedLineReactor): running analysis...", "\n");
	// Search for the following, in the appropriate order:
	//
	// Search 1: PRECISE_BORDERLINE or PRECISE_MIXED, in the HOST origin point
	if 
	(
		(fusedPointMetaRef->searchForSummaryInSpecifiedOrigin(FusedPointSummary::PRECISE_BORDERLINE, FusionCandidateOrigin::HOST).wasFound == true)
		//||
		//(fusedPointMetaRef->searchForSummaryInSpecifiedOrigin(FusedPointSummary::PRECISE_MIXED, FusionCandidateOrigin::HOST).wasFound == true)
	)
	{
		reactorBaseLogger.log("(SharedLineReactor) !! Notice: entered search 1. ", "\n");
		glm::vec3 precisePoint = fusedPointMetaRef->searchForSummaryInSpecifiedOrigin(FusedPointSummary::PRECISE_BORDERLINE, FusionCandidateOrigin::HOST).foundPoint;
		reactorBaseLogger.log("(SharedLineReactor) value of precisePoint is: ", precisePoint.x, ", ", precisePoint.y, ", ", precisePoint.z, "\n");
		SummaryScanResult otherSummary = fusedPointMetaRef->searchForOtherSummary(precisePoint);
		//std::cout << "(SharedLineReactor) Search 1-> PRECISE found in host (PRECISE_BORDERLINE or PRECISE_MIXED); processing for INTERCEPTS_POINT_PRECISE." << std::endl;
		reactorBaseLogger.log("(SharedLineReactor) Search 1-> PRECISE found in host (PRECISE_BORDERLINE or PRECISE_MIXED); processing for INTERCEPTS_POINT_PRECISE.", "\n");
		buildInterceptsPointPrecise(precisePoint, otherSummary.foundPoint);
	}

	
	else if
	(
		
		(fusedPointMetaRef->searchForSummaryInSpecifiedOrigin(FusedPointSummary::PRECISE_MIXED, FusionCandidateOrigin::HOST).wasFound == true)
	)
	{
		reactorBaseLogger.log("(SharedLineReactor) !! Notice: entered search 2. (PRECISE_MIXED)", "\n");
		glm::vec3 precisePoint = fusedPointMetaRef->searchForSummaryInSpecifiedOrigin(FusedPointSummary::PRECISE_MIXED, FusionCandidateOrigin::HOST).foundPoint;
		reactorBaseLogger.log("(SharedLineReactor) value of precisePoint is: ", precisePoint.x, ", ", precisePoint.y, ", ", precisePoint.z, "\n");
		SummaryScanResult otherSummary = fusedPointMetaRef->searchForOtherSummary(precisePoint);
		//std::cout << "(SharedLineReactor) Search 1-> PRECISE found in host (PRECISE_BORDERLINE or PRECISE_MIXED); processing for INTERCEPTS_POINT_PRECISE." << std::endl;
		reactorBaseLogger.log("(SharedLineReactor) Search 2-> PRECISE found in host (PRECISE_MIXED); processing for INTERCEPTS_POINT_PRECISE.", "\n");
		buildInterceptsPointPrecise(precisePoint, otherSummary.foundPoint);
	}
	

	else if (fusedPointMetaRef->searchForSummaryInSpecifiedOrigin(FusedPointSummary::PRECISE_NONBORDERLINE, FusionCandidateOrigin::HOST).wasFound == true)
	{
		// special case: needs to be handled by other logic.
	}

	// Search 2: TYPICAL_BORDERLINE, in the HOST origin point
	else if (fusedPointMetaRef->searchForSummaryInSpecifiedOrigin(FusedPointSummary::TYPICAL_BORDERLINE, FusionCandidateOrigin::HOST).wasFound == true)
	{
		// when only a single border line of the host that intercepts the guest and it is a TYPICAL_BORDERLINE, we know for a fact it must be a PARTIAL_BOUND.
		// It also means, we don't care about the type of the other point (Which belongs to the guest); we only need it's point.
		glm::vec3 borderPoint = fusedPointMetaRef->searchForSummaryInSpecifiedOrigin(FusedPointSummary::TYPICAL_BORDERLINE, FusionCandidateOrigin::HOST).foundPoint;
		SummaryScanResult otherSummary = fusedPointMetaRef->searchForOtherSummary(borderPoint);
		//std::cout << "(SharedLineReactor) Search 2-> TYPICAL_BORDERLINE found in host; processing for PARTIAL_BOUND " << std::endl;
		reactorBaseLogger.log("(SharedLineReactor) Search 3-> TYPICAL_BORDERLINE found in host; processing for PARTIAL_BOUND ", "\n");
		buildPartialBound(borderPoint, otherSummary.foundPoint);
	}

	// Search 3: TYPICAL_NONBORDERLINE, in the HOST origin point
	else if (fusedPointMetaRef->searchForSummaryInSpecifiedOrigin(FusedPointSummary::TYPICAL_NONBORDERLINE, FusionCandidateOrigin::HOST).wasFound == true)
	{
		glm::vec3 nonBorderPoint = fusedPointMetaRef->searchForSummaryInSpecifiedOrigin(FusedPointSummary::TYPICAL_NONBORDERLINE, FusionCandidateOrigin::HOST).foundPoint;
		SummaryScanResult otherSummary = fusedPointMetaRef->searchForOtherSummary(nonBorderPoint);
		//std::cout << "(SharedLineReactor) Search 3-> TYPICAL_NONBORDERLINE found in host; processing for NON_BOUND " << std::endl;
		reactorBaseLogger.log("(SharedLineReactor) Search 4-> TYPICAL_NONBORDERLINE found in host; processing for NON_BOUND ", "\n");
		buildNonBound(nonBorderPoint, otherSummary.foundPoint);
	}
}

void SharedLineReactor::buildInterceptsPointPrecise(glm::vec3 in_buildStartPoint, glm::vec3 in_otherPoint)
{
	//std::cout << "(SharedLineReactor): building INTERCEPTS_POINT_PRECISE..." << std::endl;
	reactorBaseLogger.log("(SharedLineReactor): building INTERCEPTS_POINT_PRECISE...", "\n");

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
		//std::cout << "Sub data at index " << x;
		//std::cout << ": isBorderLine: " << subDataArray[x].isBorderLine;
		//std::cout << ": borderLineID: " << subDataArray[x].borderLineValue << std::endl;

		reactorBaseLogger.log("(SharedLineReactor) Sub data at index ", x, "\n");
		reactorBaseLogger.log("(SharedLineReactor) : isBorderLine: ", subDataArray[x].isBorderLine, ": borderLineID: ", subDataArray[x].borderLineValue, "\n");
	}

	auto sharedLines = hostFusionAnalysisRef->sPolyRef->getBorderLinesForSharedPoint(in_buildStartPoint);
	if (reactorBaseLogger.isLoggingSet())
	{
		reactorBaseLogger.log("(SharedLineReactor): printing sharedLines for this point: ", "\n");
		auto sharedLinesBegin = sharedLines.begin();
		auto sharedLinesEnd = sharedLines.end();
		for (; sharedLinesBegin != sharedLinesEnd; sharedLinesBegin++)
		{
			std::cout << sharedLinesBegin->second << std::endl;
		}
	}

	resultantLine.type = IntersectionType::INTERCEPTS_POINT_PRECISE;
	resultantLine.line.pointA = in_buildStartPoint;							// point A is the precise point
	resultantLine.line.pointB = in_otherPoint;								// point B is the other point
	//resultantLine.line.pointABorder = subDataArray[0].borderLineValue;		// load border line values
	//resultantLine.line.pointBBorder = subDataArray[1].borderLineValue;		// "" 

	resultantLine.line.pointABorder = sharedLines[0];		// load border line values
	resultantLine.line.pointBBorder = sharedLines[1];		// "" 

	resultantLine.line.numberOfBorderLines = 1;
	resultantLine.emptyNormal = guestFusionAnalysisRef->sPolyRef->polyEmptyNormal;

	//std::cout << "(SharedLineReactor): finished producing INTERCEPT_POINTS_PRECISE, stats are: " << std::endl;
	//std::cout << "(SharedLineReactor): point A: " << resultantLine.line.pointA.x << ", " << resultantLine.line.pointA.y << ", " << resultantLine.line.pointA.z << std::endl;
	//std::cout << "(SharedLineReactor): point B: " << resultantLine.line.pointB.x << ", " << resultantLine.line.pointB.y << ", " << resultantLine.line.pointB.z << std::endl;
	//std::cout << "(SharedLineReactor): pointABorder: " << resultantLine.line.pointABorder << std::endl;
	//std::cout << "(SharedLineReactor): pointBBorder: " << resultantLine.line.pointBBorder << std::endl;
	//std::cout << "(SharedLineReactor): empty normal: " << resultantLine.emptyNormal.x << ", " << resultantLine.emptyNormal.y << ", " << resultantLine.emptyNormal.z << std::endl;

	reactorBaseLogger.log("(SharedLineReactor): finished producing INTERCEPT_POINTS_PRECISE, stats are: ", "\n");
	reactorBaseLogger.log("(SharedLineReactor): point A: ", resultantLine.line.pointA.x, ", ", resultantLine.line.pointA.y, ", ", resultantLine.line.pointA.z, "\n");
	reactorBaseLogger.log("(SharedLineReactor): point B: ", resultantLine.line.pointB.x, ", ", resultantLine.line.pointB.y, ", ", resultantLine.line.pointB.z, "\n");
	reactorBaseLogger.log("(SharedLineReactor): pointABorder: ", resultantLine.line.pointABorder, "\n");
	reactorBaseLogger.log("(SharedLineReactor): pointBBorder: ", resultantLine.line.pointBBorder, "\n");
	reactorBaseLogger.log("(SharedLineReactor): empty normal: ", resultantLine.emptyNormal.x, ", ", resultantLine.emptyNormal.y, ", ", resultantLine.emptyNormal.z, "\n");
}

void SharedLineReactor::buildPartialBound(glm::vec3 in_buildStartPoint, glm::vec3 in_otherPoint)
{
	// remember, point A of partial bound is the point that lies  on the border.
	//std::cout << "(SharedLineReactor): building PARTIAL_BOUND..." << std::endl;
	reactorBaseLogger.log("(SharedLineReactor): building PARTIAL_BOUND...", "\n");

	std::vector<FusedPointSubData>* subDataRef = hostFusionAnalysisRef->fusedPoints.fetchSubDataVectorForPoint(in_buildStartPoint);
	auto subDataRefBegin = subDataRef->begin();
	FusedPointSubData singleSubData = *subDataRefBegin;

	resultantLine.type = IntersectionType::PARTIAL_BOUND;
	resultantLine.line.pointA = in_buildStartPoint;
	resultantLine.line.pointB = in_otherPoint;
	resultantLine.line.isPointAOnBorder = 1;
	resultantLine.line.pointABorder = singleSubData.borderLineValue;
	resultantLine.line.numberOfBorderLines = 1;
	resultantLine.emptyNormal = guestFusionAnalysisRef->sPolyRef->polyEmptyNormal;

	//std::cout << "(SharedLineReactor): finished producing PARTIAL_BOUND, stats are: " << std::endl;
	//std::cout << "(SharedLineReactor): point A: " << resultantLine.line.pointA.x << ", " << resultantLine.line.pointA.y << ", " << resultantLine.line.pointA.z << std::endl;
	//std::cout << "(SharedLineReactor): point B: " << resultantLine.line.pointB.x << ", " << resultantLine.line.pointB.y << ", " << resultantLine.line.pointB.z << std::endl;
	//std::cout << "(SharedLineReactor): pointABorder: " << resultantLine.line.pointABorder << std::endl;
	//std::cout << "(SharedLineReactor): empty normal: " << resultantLine.emptyNormal.x << ", " << resultantLine.emptyNormal.y << ", " << resultantLine.emptyNormal.z << std::endl;

	reactorBaseLogger.log("(SharedLineReactor): finished producing PARTIAL_BOUND, stats are: ", "\n");
	reactorBaseLogger.log("(SharedLineReactor): point A: ", resultantLine.line.pointA.x, ", ", resultantLine.line.pointA.y, ", ", resultantLine.line.pointA.z, "\n");
	reactorBaseLogger.log("(SharedLineReactor): point B: ", resultantLine.line.pointB.x, ", ", resultantLine.line.pointB.y, ", ", resultantLine.line.pointB.z, "\n");
	reactorBaseLogger.log("(SharedLineReactor): pointABorder: ", resultantLine.line.pointABorder, "\n");
	reactorBaseLogger.log("(SharedLineReactor): empty normal: ", resultantLine.emptyNormal.x, ", ", resultantLine.emptyNormal.y, ", ", resultantLine.emptyNormal.z, "\n");
}

void SharedLineReactor::buildNonBound(glm::vec3 in_buildStartPoint, glm::vec3 in_otherPoint)
{
	reactorBaseLogger.log("(SharedLineReactor): building NON_BOUND...", "\n");

	resultantLine.type = IntersectionType::NON_BOUND;
	resultantLine.line.pointA = in_buildStartPoint;
	resultantLine.line.pointB = in_otherPoint;
	resultantLine.emptyNormal = guestFusionAnalysisRef->sPolyRef->polyEmptyNormal;

	//std::cout << "(SharedLineReactor): finished producing NON_BOUND, stats are: " << std::endl;
	//std::cout << "(SharedLineReactor): point A: " << resultantLine.line.pointA.x << ", " << resultantLine.line.pointA.y << ", " << resultantLine.line.pointA.z << std::endl;
	//std::cout << "(SharedLineReactor): point B: " << resultantLine.line.pointB.x << ", " << resultantLine.line.pointB.y << ", " << resultantLine.line.pointB.z << std::endl;
	//std::cout << "(SharedLineReactor): empty normal: " << resultantLine.emptyNormal.x << ", " << resultantLine.emptyNormal.y << ", " << resultantLine.emptyNormal.z << std::endl;

	reactorBaseLogger.log("(SharedLineReactor): finished producing NON_BOUND, stats are: ", "\n");
	reactorBaseLogger.log("(SharedLineReactor): point A: ", resultantLine.line.pointA.x, ", ", resultantLine.line.pointA.y, ", ", resultantLine.line.pointA.z, "\n");
	reactorBaseLogger.log("(SharedLineReactor): point B: ", resultantLine.line.pointB.x, ", ", resultantLine.line.pointB.y, ", ", resultantLine.line.pointB.z, "\n");
	reactorBaseLogger.log("(SharedLineReactor): empty normal: ", resultantLine.emptyNormal.x, ", ", resultantLine.emptyNormal.y, ", ", resultantLine.emptyNormal.z, "\n");
}