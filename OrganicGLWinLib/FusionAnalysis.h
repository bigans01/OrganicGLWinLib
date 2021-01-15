#pragma once

#ifndef FUSIONANALYSIS_H
#define FUSIONANALYSIS_H

#include "FusionAnalysisResult.h"
#include <map>
#include "IntersectionLine.h"
#include "FusionCandidate.h"
#include "IntersectionResult.h"

class SPoly;
class FusionAnalysis
{
	public:
		void setMapRefAndRunAnalysis(std::map<int, IntersectionLine>* in_intersectionResultMapRef);
		void insertFusionCandidate(FusionCandidate in_fusionCandidate, IntersectionResult in_intersectionResult);
		void setSPolyRef(SPoly* in_sPolyRef);
		void clearCandidateData();
		FusionAnalysisResult checkResult();
	private:
		int numberOfProcessedFusionCandidates = 0;
		SPoly* sPolyRef = nullptr;
		FusionAnalysisResult analysisResult = FusionAnalysisResult::NONE;
};

#endif 
