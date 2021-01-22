#pragma once

#ifndef FUSIONANALYSIS_H
#define FUSIONANALYSIS_H

#include "FusionAnalysisResult.h"
#include <map>
#include "IntersectionLine.h"
#include "FusionCandidate.h"
#include "IntersectionResult.h"
#include "FusionAnalysisPrimaryClassification.h"
#include "FusionAnalysisPointsClassification.h"
#include "FusionAnalysisCoplanarLineType.h"
#include <iostream>
#include "FusedPointContainer.h"
#include "FusedPointSubData.h"
#include "FusionCandidateOrigin.h"
#include "PolyLogger.h"

class SPoly;
class FusionAnalysis
{
	public:
		friend class FusedPointReactor;
		friend class HostLineReactor;
		friend class SharedLineReactor;
		friend class GuestLineReactor;
		void setMapRefAndRunAnalysis(std::map<int, IntersectionLine>* in_intersectionResultMapRef);
		void insertFusionCandidate(FusionCandidateOrigin in_fusionCandidateOrigin, int in_lineIndex, FusionCandidate in_fusionCandidate);
		void setSPolyRef(SPoly* in_sPolyRef);
		void determineClassifications();
		void clearCandidateData();
		void printClassifications();
		int getNumberOfProcessedFusionCandidates();
		FusionAnalysisResult checkResult();
		PolyLogger fusionAnalysisLogger;
	private:
		int numberOfProcessedFusionCandidates = 0;
		SPoly* sPolyRef = nullptr;
		FusedPointContainer fusedPoints;
		FusionAnalysisResult analysisResult = FusionAnalysisResult::NONE;
		FusionAnalysisPrimaryClassification primaryClass = FusionAnalysisPrimaryClassification::NONE;
		FusionAnalysisPointsClassification pointsClass = FusionAnalysisPointsClassification::NONE;

		bool coplanarLineFlag = false;	// set this to true when inserting a FusionCandidate that is coplanar to the compared-to triangle.
		FusionAnalysisCoplanarLineType coplanarType = FusionAnalysisCoplanarLineType::NONE;
};

#endif 
