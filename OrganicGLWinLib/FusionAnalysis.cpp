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

void FusionAnalysis::insertFusionCandidate(int in_lineIndex, FusionCandidate in_fusionCandidate, IntersectionResult in_intersectionResult)
{
	numberOfProcessedFusionCandidates++;
	std::cout << "~~~~~~~~~~~~~~ Fusion candidate, point: " << in_fusionCandidate.candidateIntersectionResult.intersectedPoint.x << ", "
		<< in_fusionCandidate.candidateIntersectionResult.intersectedPoint.y << ", "
		<< in_fusionCandidate.candidateIntersectionResult.intersectedPoint.z << std::endl;

	std::cout << "~~~~~ Is border line: " << in_fusionCandidate.candidateIntersectionResult.wasIntersectOnBorderLine << std::endl;


	// first, check if it's parallel.
	if
	(
		(in_fusionCandidate.candidateIntersectionResult.wasIntersectFound == 2)	// it was within triangle plane,
		||																		// -OR- 
		(in_fusionCandidate.candidateIntersectionResult.wasIntersectFound == 3)	// it was equal to a line exactly
	)
	{
		coplanarLineFlag = true;	// coplanar line flag as being found ("true")

		// if it is a border line, flag it
		if (in_fusionCandidate.candidateIntersectionResult.wasIntersectOnBorderLine == 0)
		{
			coplanarType = FusionAnalysisCoplanarLineType::COPLANAR_NONBORDERLINE;
		}
		else if (in_fusionCandidate.candidateIntersectionResult.wasIntersectOnBorderLine == 1)
		{
			coplanarType = FusionAnalysisCoplanarLineType::COPLANAR_BORDERLINE;
		}

	}
	else if
	(
		(in_fusionCandidate.candidateIntersectionResult.wasIntersectFound == 1)	// a typical intercept
	)
	{
		FusedPointSubData newSubData(in_lineIndex, in_fusionCandidate.candidateIntersectionResult.wasIntersectOnBorderLine, in_fusionCandidate.candidateIntersectionResult.borderLineID);
		fusedPoints.insertSubDataForPoint(in_intersectionResult.intersectedPoint, newSubData);
		std::cout << ">>>>>>> inserted sub data for point... " << std::endl;
		int waitVal = 3;
		std::cin >> waitVal;
	}
	std::cout << "####### number of processed fusion candidates is now: " << numberOfProcessedFusionCandidates << std::endl;
}

void FusionAnalysis::setSPolyRef(SPoly* in_sPolyRef)
{
	sPolyRef = in_sPolyRef;
}

void FusionAnalysis::determineClassifications()
{
	// determine the primary classification.
	if (numberOfProcessedFusionCandidates == 0)
	{
		primaryClass = FusionAnalysisPrimaryClassification::NONE;
	}
	else if (numberOfProcessedFusionCandidates == 1)
	{
		primaryClass = FusionAnalysisPrimaryClassification::SINGLE;
	}
	else if (numberOfProcessedFusionCandidates == 2)
	{
		primaryClass = FusionAnalysisPrimaryClassification::DOUBLE;
	}
	else if (numberOfProcessedFusionCandidates == 3)
	{
		primaryClass = FusionAnalysisPrimaryClassification::TRIPLE;
	}
}

FusionAnalysisResult FusionAnalysis::checkResult()
{
	return analysisResult;
}

void FusionAnalysis::clearCandidateData()
{
	numberOfProcessedFusionCandidates = 0;
	coplanarLineFlag = false;
	coplanarType = FusionAnalysisCoplanarLineType::NONE;
	primaryClass = FusionAnalysisPrimaryClassification::NONE;
	fusedPoints.clearFusedPoints();

	std::cout << "############################## ->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> cleared candidate data. ############################ ->>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
}

void FusionAnalysis::printClassifications()
{
	std::cout << "###############////////// printing classifications: " << std::endl;
	std::cout << "Primary: ";

	if (primaryClass == FusionAnalysisPrimaryClassification::NONE)
	{
		std::cout << "NONE" << std::endl;
	}
	else if (primaryClass == FusionAnalysisPrimaryClassification::SINGLE)
	{
		std::cout << "SINGLE" << std::endl;
	}
	else if (primaryClass == FusionAnalysisPrimaryClassification::DOUBLE)
	{
		std::cout << "DOUBLE" << std::endl;
	}
	else if (primaryClass == FusionAnalysisPrimaryClassification::TRIPLE)
	{
		std::cout << "TRIPLE" << std::endl;
	}

	std::cout << "Coplanar flag: ";
	if (coplanarLineFlag == false)
	{
		std::cout << "false" << std::endl;
	}
	else if (coplanarLineFlag == true)
	{
		std::cout << "true" << std::endl;
	}

	std::cout << "Coplanar type: ";
	if (coplanarType == FusionAnalysisCoplanarLineType::NONE)
	{
		std::cout << "NONE" << std::endl;
	}
	else if (coplanarType == FusionAnalysisCoplanarLineType::COPLANAR_BORDERLINE)
	{
		std::cout << "COPLANAR_BORDERLINE" << std::endl;
	}
	else if (coplanarType == FusionAnalysisCoplanarLineType::COPLANAR_NONBORDERLINE)
	{
		std::cout << "COPLANAR_NONBORDERLINE" << std::endl;
	}

	fusedPoints.printFusedPoints();
}

int FusionAnalysis::getNumberOfProcessedFusionCandidates()
{
	return numberOfProcessedFusionCandidates;
}