#include "stdafx.h"
#include "FusionAnalysis.h"
#include "SPoly.h"

void FusionAnalysis::setMapRefAndRunAnalysis(std::map<int, IntersectionLine>* in_intersectionResultMapRef)
{
	//intersectionMapRef = in_intersectionResultMapRef;
	if ((*in_intersectionResultMapRef).size() == 3)
	{
		analysisResult = FusionAnalysisResult::PRECISE;
	}
}

void FusionAnalysis::insertFusionCandidate(FusionCandidate in_fusionCandidate, IntersectionResult in_intersectionResult)
{
	numberOfProcessedFusionCandidates++;
}

void FusionAnalysis::setSPolyRef(SPoly* in_sPolyRef)
{
	sPolyRef = in_sPolyRef;
}

FusionAnalysisResult FusionAnalysis::checkResult()
{
	return analysisResult;
}

void FusionAnalysis::clearCandidateData()
{

}