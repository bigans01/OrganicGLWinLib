#include "stdafx.h"
#include "FusedPointReactor.h"

void FusedPointReactor::igniteReaction()
{
	runValidityTests();

	// if the validity tests passed, begin analysis of the categorized line type.
	if (fusionContinuationFlag == true)
	{
		runCategorizedLineBaseTypeAnalysis();
	}
}

void FusedPointReactor::runValidityTests()
{
	// run test 1: if the host FusionAnalysis' coplanarType is set to
	// FusionAnalysisCoplanarLineType::COPLANAR_BORDERLINE, there is 
	// no point in continuing.
	if (hostFusionAnalysisRef->coplanarType == FusionAnalysisCoplanarLineType::COPLANAR_BORDERLINE) 
	{
		fusionContinuationFlag = false;
		std::cout << "(Reactor): Test 1 (check to ensure host FusionAnalysis isn't set to COPLANAR_BORDERLINE) FAILED. " << std::endl;
	}
	else
	{
		std::cout << "(Reactor): Test 1 (check to ensure host FusionAnalysis isn't set to COPLANAR_BORDERLINE) PASSED. " << std::endl;
		runPointCountTest();				// run test 2.
		if (fusionContinuationFlag == true)
		{
			runPointAcquisitionAndPointUniquenessTest(); // run test 3, only if test 2 passed.
		}
	}
}

void FusedPointReactor::runPointCountTest()
{
	int hostFusionPointCount = hostFusionAnalysisRef->getNumberOfProcessedFusionCandidates();
	int guestFusionPointCount = hostFusionAnalysisRef->getNumberOfProcessedFusionCandidates();
	int totalPoints = hostFusionPointCount + guestFusionPointCount;
	if (totalPoints < 2)		// if there aren't at least 2 points between both sets, we know for certain that there is no way a line can be produced.
								// So set the valid result flag to FALSE.
	{
		fusionContinuationFlag = false;
		std::cout << "(Reactor): Test 2 (runPointCountTest) FAILED. " << std::endl;
	}
	else
	{
		std::cout << "(Reactor): Test 2 (runPointCountTest) PASSED. " << std::endl;
	}
}

void FusedPointReactor::runPointAcquisitionAndPointUniquenessTest()
{
	// load all points, from both FusedAnalysis instance's FusedPointContainer into the reactor's fused point container.
	hostFusionAnalysisRef->fusedPoints.loadPointsIntoOtherContainer(&reactorPointContainer);
	guestFusionAnalysisRef->fusedPoints.loadPointsIntoOtherContainer(&reactorPointContainer);
	if (reactorPointContainer.fusedPointMap.size() == 1)	// if there's only one point in the resulting container, flag it.
	{
		fusionContinuationFlag = false;
		std::cout << "(Reactor): Test 3 (runPointAcquisitionAndPointUniquenessTest) FAILED. " << std::endl;
	}
	else
	{
		std::cout << "(Reactor): Test 3 (runPointAcquisitionAndPointUniquenessTest) PASSED. " << std::endl;
	}
}

void FusedPointReactor::runCategorizedLineBaseTypeAnalysis()
{
	// determine which branch to use:
	// 1.) all points we need are in the host group
	// 2.) the host group has one point, the guest group has >= 1
	// 3.) the host group has no points, which means all points are in the guest

	// CASE 1: we can determine what we need from the host group alone (no reliance on guest group needed)
	if (hostFusionAnalysisRef->fusedPoints.fusedPointMap.size() == 2)
	{
		FusedPointMetaPair hostPair;
		auto hostFusionPointsBegin = hostFusionAnalysisRef->fusedPoints.fusedPointMap.begin();
		auto hostFusionPointsEnd = hostFusionAnalysisRef->fusedPoints.fusedPointMap.end();
		for (; hostFusionPointsBegin != hostFusionPointsEnd; hostFusionPointsBegin++)
		{
			glm::vec3 currentPointToSearch = hostFusionPointsBegin->second.point;
			FusedPointMeta currentPointMeta = hostFusionAnalysisRef->fusedPoints.retrieveFusedPointMeta(currentPointToSearch, FusionCandidateOrigin::HOST);
			hostPair.insertFusedPointMeta(currentPointMeta);
		}
		hostPair.printSummaries();
		HostLineReactor hostReactor(hostFusionAnalysisRef, guestFusionAnalysisRef, &hostPair);
	}

	// CASE 2: the host group only has one point; because the validity tests passed at this point, the guest group will have at least 1 point always.
	else if (hostFusionAnalysisRef->fusedPoints.fusedPointMap.size() == 1)
	{

	}

	// CASE 3: the host STriangle "engulfed" the guest STriangle; the host STriangle has no points, but the guest will always have 2.
	else if (guestFusionAnalysisRef->fusedPoints.fusedPointMap.size() == 2)
	{

	}
}
