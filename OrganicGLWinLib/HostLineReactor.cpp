#include "stdafx.h"
#include "HostLineReactor.h"

void HostLineReactor::runAnalysis()
{
	//std::cout << "(HostLineReactor): running analysis..." << std::endl;
	reactorBaseLogger.log("(HostLineReactor): running analysis...", "\n");

	// Search for the following, in the appropriate order:
	//
	// Search 1: PRECISE_BORDERLINE
	if (fusedPointMetaRef->searchForSummary(FusedPointSummary::PRECISE_BORDERLINE).wasFound == true)
	{
		glm::vec3 precisePoint = fusedPointMetaRef->searchForSummary(FusedPointSummary::PRECISE_BORDERLINE).foundPoint;
		SummaryScanResult otherSummary = fusedPointMetaRef->searchForOtherSummary(precisePoint);
		if (otherSummary.pointSummary == FusedPointSummary::TYPICAL_NONBORDERLINE)	// we have a PRECISE_BORDERLINE point, followed by the other point being nonbound; 
																				// this must be a PARTIAL_BOUND.
		{
			//std::cout << "(HostLineReactor) Search 2-> TYPICAL_NONBORDERLINE found; processing for INTERCEPTS_POINT_PRECISE." << std::endl;
			reactorBaseLogger.log("(HostLineReactor) Search 1-> TYPICAL_NONBORDERLINE found as otherSummary; processing for INTERCEPTS_POINT_PRECISE.", "\n");
			buildInterceptsPointPrecise(precisePoint, otherSummary.foundPoint);
		}
		else if (otherSummary.pointSummary == FusedPointSummary::TYPICAL_BORDERLINE)
		{
			//std::cout << "(HostLineReactor) Search 2-> TYPICAL_BORDERLINE found; processing for A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE." << std::endl;
			reactorBaseLogger.log("(HostLineReactor) Search 1-> TYPICAL_BORDERLINE found as otherSummary; processing for A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE.", "\n");
			buildASliceSingleInterceptsPointPrecise(precisePoint, otherSummary.foundPoint);
		}
		/*
		else if (otherSummary.pointSummary == FusedPointSummary::PRECISE_MIXED)
		{
			std::cout << "(HostLineReactor) Search 2-> PRECISE_MIXED found as otherSummary; processing for A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE." << std::endl;
		}
		*/
	}

	// Search 2: PRECISE_MIXED
	else if (fusedPointMetaRef->searchForSummary(FusedPointSummary::PRECISE_MIXED).wasFound == true)
	{
		std::cout << "(HostLineReactor) Search 2 -> PRECISE_MIXED found; processing for " << std::endl;
		glm::vec3 precisePoint = fusedPointMetaRef->searchForSummary(FusedPointSummary::PRECISE_MIXED).foundPoint;
		SummaryScanResult otherSummary = fusedPointMetaRef->searchForOtherSummary(precisePoint);
		if (otherSummary.pointSummary == FusedPointSummary::TYPICAL_BORDERLINE)
		{
			//std::cout << "(HostLineReactor) Search 2-> TYPICAL_BORDERLINE found; processing for A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE." << std::endl;
			reactorBaseLogger.log("(HostLineReactor) Search 2-> TYPICAL_BORDERLINE found as otherSummary; processing for A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE.", "\n");
			buildASliceSingleInterceptsPointPrecise(precisePoint, otherSummary.foundPoint);
		}
	}

	// Search 3: PRECISE_NONBORDERLINE
	else if (fusedPointMetaRef->searchForSummary(FusedPointSummary::PRECISE_NONBORDERLINE).wasFound == true)
	{

	}

	// Search 4: TYPICAL_BORDERLINE
	else if (fusedPointMetaRef->searchForSummary(FusedPointSummary::TYPICAL_BORDERLINE).wasFound == true)
	{
		glm::vec3 borderPoint = fusedPointMetaRef->searchForSummary(FusedPointSummary::TYPICAL_BORDERLINE).foundPoint;
		SummaryScanResult otherSummary = fusedPointMetaRef->searchForOtherSummary(borderPoint);
		if (otherSummary.pointSummary == FusedPointSummary::TYPICAL_BORDERLINE) // the first point is a TYPICAL_BORDERLINE, and so is this one; it's a SLICE.
		{
			//std::cout << "(HostLineReactor) Search 3 -> TYPICAL_BORDERLINE found; processing for A_SLICE. " << std::endl;
			reactorBaseLogger.log("(HostLineReactor) Search 3 -> TYPICAL_BORDERLINE found; processing for A_SLICE. ", "\n");
			buildASlice(borderPoint, otherSummary.foundPoint);
		}
		else if (otherSummary.pointSummary == FusedPointSummary::TYPICAL_NONBORDERLINE) // the first point is a TYPICAL_BORDERLINE, but this one is TYPICAL_NONBORDERLINE; it's a PARTIAL_BOUND.
		{
			//std::cout << "(HostLineReactor) Search 3 -> TYPICAL_NONBORDERLINE found; processing for PARTIAL_BOUND. " << std::endl;
			reactorBaseLogger.log("(HostLineReactor) Search 3 -> TYPICAL_NONBORDERLINE found; processing for PARTIAL_BOUND. ", "\n");
			buildPartialBound(borderPoint, otherSummary.foundPoint);
		}
	}

	// Search 5: TYPICAL_NONBORDERLINE
	else if (fusedPointMetaRef->searchForSummary(FusedPointSummary::TYPICAL_NONBORDERLINE).wasFound == true)
	{
		glm::vec3 firstNonBorderPoint = fusedPointMetaRef->searchForSummary(FusedPointSummary::TYPICAL_NONBORDERLINE).foundPoint;
		SummaryScanResult otherSummary = fusedPointMetaRef->searchForOtherSummary(firstNonBorderPoint);

		//std::cout << "******************* NOTICE: typical non_border line found. " << std::endl;
		reactorBaseLogger.log("(HostLineReactor): building NON_BOUND...", "\n");
		buildNonBound(firstNonBorderPoint, otherSummary.foundPoint);
	}
}

void HostLineReactor::buildInterceptsPointPrecise(glm::vec3 in_buildStartPoint, glm::vec3 in_otherPoint)
{
	//std::cout << "(HostLineReactor): building INTERCEPTS_POINT_PRECISE..." << std::endl;
	reactorBaseLogger.log("(HostLineReactor): building INTERCEPTS_POINT_PRECISE...", "\n");

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
		reactorBaseLogger.log("(HostLineReactor) Sub data at index ", x, "\n");
		reactorBaseLogger.log("(HostLineReactor) : isBorderLine: ", subDataArray[x].isBorderLine, "\n");
		reactorBaseLogger.log("(HostLineReactor) : borderLineID: ", subDataArray[x].borderLineValue, "\n");
	}

	resultantLine.type = IntersectionType::INTERCEPTS_POINT_PRECISE;
	resultantLine.line.pointA = in_buildStartPoint;							// point A is the precise point
	resultantLine.line.pointB = in_otherPoint;								// point B is the other point
	resultantLine.line.pointABorder = subDataArray[0].borderLineValue;		// load border line values
	resultantLine.line.pointBBorder = subDataArray[1].borderLineValue;		// "" 
	resultantLine.line.numberOfBorderLines = 1;
	resultantLine.emptyNormal = guestFusionAnalysisRef->sPolyRef->polyEmptyNormal;

	//std::cout << "(HostLineReactor): finished producing INTERCEPT_POINTS_PRECISE, stats are: " << std::endl;
	//std::cout << "(HostLineReactor): point A: " << resultantLine.line.pointA.x << ", " << resultantLine.line.pointA.y << ", " << resultantLine.line.pointA.z << std::endl;
	//std::cout << "(HostLineReactor): point B: " << resultantLine.line.pointB.x << ", " << resultantLine.line.pointB.y << ", " << resultantLine.line.pointB.z << std::endl;
	//std::cout << "(HostLineReactor): pointABorder: " << resultantLine.line.pointABorder << std::endl;
	//std::cout << "(HostLineReactor): pointBBorder: " << resultantLine.line.pointBBorder << std::endl;
	//std::cout << "(HostLineReactor): empty normal: " << resultantLine.emptyNormal.x << ", " << resultantLine.emptyNormal.y << ", " << resultantLine.emptyNormal.z << std::endl;

	reactorBaseLogger.log("(HostLineReactor): finished producing INTERCEPT_POINTS_PRECISE, stats are: ", "\n");
	reactorBaseLogger.log("(HostLineReactor): point A: ", resultantLine.line.pointA.x, ", ", resultantLine.line.pointA.y, ", ", resultantLine.line.pointA.z, "\n");
	reactorBaseLogger.log("(HostLineReactor): point B: ", resultantLine.line.pointB.x, ", ", resultantLine.line.pointB.y, ", ", resultantLine.line.pointB.z, "\n");
	reactorBaseLogger.log("(HostLineReactor): pointABorder: ", resultantLine.line.pointABorder, "\n");
	reactorBaseLogger.log("(HostLineReactor): pointBBorder: ", resultantLine.line.pointBBorder, "\n");
	reactorBaseLogger.log("(HostLineReactor) : empty normal : ", resultantLine.emptyNormal.x, ", ", resultantLine.emptyNormal.y, ", ", resultantLine.emptyNormal.z, "\n");
}

void HostLineReactor::buildASliceSingleInterceptsPointPrecise(glm::vec3 in_buildPrecisePoint, glm::vec3 in_otherPoint)
{
	reactorBaseLogger.log("(HostLineReactor): building A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE...", "\n");

	//std::cout << "(HostLineReactor) : building A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE..." << std::endl;
	//std::cout << "(HostLineReactor): point used to fetch subDataArray is: " << in_buildPrecisePoint.x << ", " << in_buildPrecisePoint.y << ", " << in_buildPrecisePoint.z << std::endl;

	// NEW: rebuild the host sub data.
	//
	// It was discovered on 8/4/2022, that there is chance that both of the sub data entries for a point are not necessarily registered as border lines.
	// This was causing a sudden crash, as this function expects both entries in the FusedPointSubData retreived to both be valid border lines
	// that share the point, when the point is the in_buildPrecisePoint. 
	//
	// In order to fix this anomaly, this function needs to make sure that both sub-data entries in the FusedPointSubData are border lines.
	// We can just use the value returned by  hostFusionAnalysisRef->sPolyRef->getBorderLinesForSharedPoint(in_buildPrecisePoint) below
	// to figure out what these border line ID values are; this function will return exactly 2 border lines for our selected point, 
	// and guarantee that there is no crash that occurs when the A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE branch in CategorizedLine::determineCyclingDirection 
	// ends up calling CategorizedLine::generateCyclingDirectionForASliceSingleInterceptPointPrecise, when extraData is present in the CategorizedLine.
	std::map<int, int> generatedLineMap = hostFusionAnalysisRef->sPolyRef->getBorderLinesForSharedPoint(in_buildPrecisePoint);
	hostFusionAnalysisRef->fusedPoints.rebuildSubDataForPointForTwoBorderLines(in_buildPrecisePoint, generatedLineMap);

	std::vector<FusedPointSubData>* subDataRef = hostFusionAnalysisRef->fusedPoints.fetchSubDataVectorForPoint(in_buildPrecisePoint);
	FusedPointSubData subDataArray[2];
	std::cout << "(HostLineReactor): Size of subDataRef'd vector: " << subDataRef->size() << std::endl;
	auto subDataRefBegin = subDataRef->begin();
	auto subDataRefEnd = subDataRef->end();
	int beginIndex = 0;
	for (; subDataRefBegin != subDataRefEnd; subDataRefBegin++)
	{
		subDataArray[beginIndex++] = *subDataRefBegin;
	}
	// testing, print out the contents of the sub data, by that vector's size.
	for (int x = 0; x < subDataRef->size(); x++)
	{
		std::cout << "Sub data at index " << x;
		std::cout << " triangleLineIndex: " << subDataArray[x].triangleLineIndex;
		std::cout << " | isBorderLine: " << subDataArray[x].isBorderLine;
		std::cout << " | borderLineID: " << subDataArray[x].borderLineValue;

		std::string originString = subDataArray[x].fetchOriginString();

		std::cout << " | origin: " << originString << std::endl;
		reactorBaseLogger.log("(HostLineReactor) Sub data at index ", x, "\n");
		reactorBaseLogger.log("(HostLineReactor) : isBorderLine: ", subDataArray[x].isBorderLine, "\n");
		reactorBaseLogger.log("(HostLineReactor) : borderLineID: ", subDataArray[x].borderLineValue, "\n");
	}

	// need to get border line ID from the other point.
	std::cout << "(HostLineReactor): point used to fetch subDataArray is: " << in_otherPoint.x << ", " << in_otherPoint.y << ", " << in_otherPoint.z << std::endl;
	std::vector<FusedPointSubData>* otherPointSubDataRef = hostFusionAnalysisRef->fusedPoints.fetchSubDataVectorForPoint(in_otherPoint);
	std::cout << "(HostLineReactor): Size of otherPointSubDataRef 'd vector: " << otherPointSubDataRef->size() << std::endl;
	auto otherPointSubDataRefBegin = otherPointSubDataRef->begin();
	FusedPointSubData otherSingleSubData = *otherPointSubDataRefBegin;

	FusedPointSubData otherSingleArray[2];
	auto otherDataRefBegin = otherPointSubDataRef->begin();
	auto otherDataRefEnd = otherPointSubDataRef->end();
	int otherBeginindex = 0;
	for (; otherDataRefBegin != otherDataRefEnd; otherDataRefBegin++)
	{
		otherSingleArray[otherBeginindex++] = *otherDataRefBegin;
	}
	// print out other sub data
	for (int x = 0; x < otherPointSubDataRef->size(); x++)
	{
		std::cout << "OTHER Sub data at index " << x;
		std::cout << " triangleLineIndex: " << otherSingleArray[x].triangleLineIndex;
		std::cout << " | isBorderLine: " << otherSingleArray[x].isBorderLine;
		std::cout << " | borderLineID: " << otherSingleArray[x].borderLineValue;

		std::string originString = otherSingleArray[x].fetchOriginString();

		std::cout << " | origin: " << originString << std::endl;

		reactorBaseLogger.log("(HostLineReactor) Sub data at index ", x, "\n");
		reactorBaseLogger.log("(HostLineReactor) : isBorderLine: ", otherSingleArray[x].isBorderLine, "\n");
		reactorBaseLogger.log("(HostLineReactor) : borderLineID: ", otherSingleArray[x].borderLineValue, "\n");
	}
	

	resultantLine.type = IntersectionType::A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE;
	resultantLine.line.pointA = in_buildPrecisePoint;							// point A is the precise point
	resultantLine.line.isPointAOnBorder = 1;								
	resultantLine.line.pointB = in_otherPoint;								// point B is the other point
	resultantLine.line.isPointBOnBorder = 1;
	resultantLine.line.pointBBorder = otherSingleSubData.borderLineValue;	// the border line ID for the other non-precise point of the slice line
	resultantLine.createCategorizedLineOptionals();							// must be called to fill the smart pointer data.
	resultantLine.insertOptionalDataForPoint(IRPointType::POINT_A, *subDataRef);	// fill point A of the optional data w/ the fused point data.
	resultantLine.line.numberOfBorderLines = 2;
	resultantLine.emptyNormal = guestFusionAnalysisRef->sPolyRef->polyEmptyNormal;


}

void HostLineReactor::buildPartialBound(glm::vec3 in_buildStartPoint, glm::vec3 in_otherPoint)
{
	//std::cout << "(HostLineReactor): building PARTIAL_BOUND..." << std::endl;
	reactorBaseLogger.log("(HostLineReactor): building PARTIAL_BOUND...", "\n");

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

	//std::cout << "(HostLineReactor): finished producing PARTIAL_BOUND, stats are: " << std::endl;
	//std::cout << "(HostLineReactor): point A: " << resultantLine.line.pointA.x << ", " << resultantLine.line.pointA.y << ", " << resultantLine.line.pointA.z << std::endl;
	//std::cout << "(HostLineReactor): point B: " << resultantLine.line.pointB.x << ", " << resultantLine.line.pointB.y << ", " << resultantLine.line.pointB.z << std::endl;
	//std::cout << "(HostLineReactor): pointABorder: " << resultantLine.line.pointABorder << std::endl;
	//std::cout << "(HostLineReactor): empty normal: " << resultantLine.emptyNormal.x << ", " << resultantLine.emptyNormal.y << ", " << resultantLine.emptyNormal.z << std::endl;

	reactorBaseLogger.log("(HostLineReactor): finished producing PARTIAL_BOUND, stats are: ", "\n");
	reactorBaseLogger.log("(HostLineReactor): point A: ", resultantLine.line.pointA.x, ", ", resultantLine.line.pointA.y, ", ", resultantLine.line.pointA.z, "\n");
	reactorBaseLogger.log("(HostLineReactor): point B: ", resultantLine.line.pointB.x, ", ", resultantLine.line.pointB.y, ", ", resultantLine.line.pointB.z, "\n");
	reactorBaseLogger.log("(HostLineReactor): pointABorder: ", resultantLine.line.pointABorder, "\n");
	reactorBaseLogger.log("(HostLineReactor) : empty normal : ", resultantLine.emptyNormal.x, ", ", resultantLine.emptyNormal.y, ", ", resultantLine.emptyNormal.z, "\n");
}

void HostLineReactor::buildASlice(glm::vec3 in_buildStartPoint, glm::vec3 in_otherPoint)
{
	reactorBaseLogger.log("(HostLineReactor): building A_SLICE...", "\n");
	std::vector<FusedPointSubData>* startPointsubDataRef = hostFusionAnalysisRef->fusedPoints.fetchSubDataVectorForPoint(in_buildStartPoint);
	std::vector<FusedPointSubData>* otherPointSubDataRef = hostFusionAnalysisRef->fusedPoints.fetchSubDataVectorForPoint(in_otherPoint);

	auto startPointSubDataRefBegin = startPointsubDataRef->begin();
	auto otherPointSubDataRefBegin = otherPointSubDataRef->begin();

	FusedPointSubData startSingleSubData = *startPointSubDataRefBegin;
	FusedPointSubData otherSingleSubData = *otherPointSubDataRefBegin;

	resultantLine.type = IntersectionType::A_SLICE;
	resultantLine.line.pointA = in_buildStartPoint;
	resultantLine.line.isPointAOnBorder = 1;
	resultantLine.line.pointABorder = startSingleSubData.borderLineValue;
	resultantLine.line.pointB = in_otherPoint;
	resultantLine.line.isPointBOnBorder = 1;
	resultantLine.line.pointBBorder = otherSingleSubData.borderLineValue;
	resultantLine.line.numberOfBorderLines = 2;
	resultantLine.emptyNormal = guestFusionAnalysisRef->sPolyRef->polyEmptyNormal;

	//std::cout << "(HostLineReactor): finished producing A_SLICE, stats are: " << std::endl;
	//std::cout << "(HostLineReactor): point A: " << resultantLine.line.pointA.x << ", " << resultantLine.line.pointA.y << ", " << resultantLine.line.pointA.z << std::endl;
	//std::cout << "(HostLineReactor): isPointAOnBorder: " << resultantLine.line.isPointAOnBorder << std::endl;
	//std::cout << "(HostLineReactor): pointABorder: " << resultantLine.line.pointABorder << std::endl;
	//std::cout << "(HostLineReactor): point B: " << resultantLine.line.pointB.x << ", " << resultantLine.line.pointB.y << ", " << resultantLine.line.pointB.z << std::endl;
	//std::cout << "(HostLineReactor): isPointBOnBorder: " << resultantLine.line.isPointBOnBorder << std::endl;
	//std::cout << "(HostLineReactor): pointBBorder: " << resultantLine.line.pointBBorder << std::endl;
	//std::cout << "(HostLineReactor): empty normal: " << resultantLine.emptyNormal.x << ", " << resultantLine.emptyNormal.y << ", " << resultantLine.emptyNormal.z << std::endl;

	reactorBaseLogger.log("(HostLineReactor): finished producing A_SLICE, stats are: ", "\n");
	reactorBaseLogger.log("(HostLineReactor): point A: ", resultantLine.line.pointA.x, ", ", resultantLine.line.pointA.y, ", ", resultantLine.line.pointA.z, "\n");
	reactorBaseLogger.log("(HostLineReactor): isPointAOnBorder: ", resultantLine.line.isPointAOnBorder, "\n");
	reactorBaseLogger.log("(HostLineReactor): pointABorder: ", resultantLine.line.pointABorder, "\n");
	reactorBaseLogger.log("(HostLineReactor): point B: ", resultantLine.line.pointB.x, ", ", resultantLine.line.pointB.y, ", ", resultantLine.line.pointB.z, "\n");
	reactorBaseLogger.log("(HostLineReactor): isPointBOnBorder: ", resultantLine.line.isPointBOnBorder, "\n");
	reactorBaseLogger.log("(HostLineReactor): pointBBorder: ", resultantLine.line.pointBBorder, "\n");
	reactorBaseLogger.log("(HostLineReactor): empty normal: ", resultantLine.emptyNormal.x, ", ", resultantLine.emptyNormal.y, ", ", resultantLine.emptyNormal.z, "\n");

}

void HostLineReactor::buildNonBound(glm::vec3 in_buildStartPoint, glm::vec3 in_otherPoint)
{
	reactorBaseLogger.log("(HostLineReactor) !!!!!!!! building NON_BOUND line. ", "\n");
	
	//auto firstPoint = guestFusionAnalysisRef->sPolyRef->polyEmptyNormal;
	resultantLine.type = IntersectionType::NON_BOUND;
	resultantLine.line.pointA = in_buildStartPoint;
	resultantLine.line.pointB = in_otherPoint;
	resultantLine.emptyNormal = guestFusionAnalysisRef->sPolyRef->polyEmptyNormal;

	reactorBaseLogger.log("(GuestLineReactor): finished producing NON_BOUND, stats are: ", "\n");
	reactorBaseLogger.log("(GuestLineReactor): point A: ", resultantLine.line.pointA.x, ", ", resultantLine.line.pointA.y, ", ", resultantLine.line.pointA.z, "\n");
	reactorBaseLogger.log("(GuestLineReactor): point B: ", resultantLine.line.pointB.x, ", ", resultantLine.line.pointB.y, ", ", resultantLine.line.pointB.z, "\n");
	reactorBaseLogger.log("(GuestLineReactor): empty normal: ", resultantLine.emptyNormal.x, ", ", resultantLine.emptyNormal.y, ", ", resultantLine.emptyNormal.z, "\n");
}