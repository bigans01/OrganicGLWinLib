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
		//std::cout << "(Reactor): Test 1 (check to ensure host FusionAnalysis isn't set to COPLANAR_BORDERLINE) FAILED. " << std::endl;
		fusedPointReactorLogger.log("(Reactor): Test 1 (check to ensure host FusionAnalysis isn't set to COPLANAR_BORDERLINE) FAILED. ", "\n");
	}
	else
	{
		//std::cout << "(Reactor): Test 1 (check to ensure host FusionAnalysis isn't set to COPLANAR_BORDERLINE) PASSED. " << std::endl;
		fusedPointReactorLogger.log("(Reactor): Test 1 (check to ensure host FusionAnalysis isn't set to COPLANAR_BORDERLINE) PASSED. ", "\n");
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
	int guestFusionPointCount = guestFusionAnalysisRef->getNumberOfProcessedFusionCandidates();
	int totalPoints = hostFusionPointCount + guestFusionPointCount;
	if (totalPoints < 2)		// if there aren't at least 2 points between both sets, we know for certain that there is no way a line can be produced.
								// So set the valid result flag to FALSE.
	{
		fusionContinuationFlag = false;
		//std::cout << "(Reactor): Test 2 (runPointCountTest) FAILED. " << std::endl;
		fusedPointReactorLogger.log("(Reactor): Test 2 (runPointCountTest) FAILED. ", "\n");
	}
	else
	{
		//std::cout << "(Reactor): Test 2 (runPointCountTest) PASSED. " << std::endl;
		fusedPointReactorLogger.log("(Reactor): Test 2 (runPointCountTest) PASSED. ", "\n");
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
		//std::cout << "(Reactor): Test 3 (runPointAcquisitionAndPointUniquenessTest) FAILED. " << std::endl;
		fusedPointReactorLogger.log("(Reactor): Test 3 (runPointAcquisitionAndPointUniquenessTest) FAILED. ", "\n");
	}
	else
	{
		//std::cout << "(Reactor): Test 3 (runPointAcquisitionAndPointUniquenessTest) PASSED. " << std::endl;
		fusedPointReactorLogger.log("(Reactor): Test 3 (runPointAcquisitionAndPointUniquenessTest) PASSED. ", "\n");
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
		//std::cout << "(Reactor) Running logic for CASE 1: (Host) " << std::endl;
		fusedPointReactorLogger.log("(Reactor) Running logic for CASE 1: (Host) ", "\n");
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
		HostLineReactor hostReactor(hostFusionAnalysisRef, guestFusionAnalysisRef, &hostPair, fusedPointReactorDebugLevel);
		producedLine = hostReactor.resultantLine;
	}

	// CASE 2: the host group only has one point; because the validity tests passed at this point, the guest group will have at least 1 point always.
	else if (hostFusionAnalysisRef->fusedPoints.fusedPointMap.size() == 1)
	{
		//std::cout << "(Reactor) Running logic for CASE 2: (Shared) " << std::endl;
		fusedPointReactorLogger.log("(Reactor) Running logic for CASE 2: (Shared) ", "\n");
		FusedPointMetaPair sharedPair;

		// get the first point from the host group
		auto hostFusionPointsBegin = hostFusionAnalysisRef->fusedPoints.fusedPointMap.begin();
		glm::vec3 currentPointToSearch = hostFusionPointsBegin->second.point;
		FusedPointMeta hostPointMeta = hostFusionAnalysisRef->fusedPoints.retrieveFusedPointMeta(currentPointToSearch, FusionCandidateOrigin::HOST);
		sharedPair.insertFusedPointMeta(hostPointMeta);

		// get the other point from the guest group
		auto guestFusionPointsBegin = guestFusionAnalysisRef->fusedPoints.fusedPointMap.begin();
		FusedPointMeta guestPointMeta = guestFusionAnalysisRef->fusedPoints.retrieveOtherFusedPointMeta(currentPointToSearch, FusionCandidateOrigin::GUEST);
		sharedPair.insertFusedPointMeta(guestPointMeta);

		sharedPair.printSummaries();
		SharedLineReactor sharedReactor(hostFusionAnalysisRef, guestFusionAnalysisRef, &sharedPair, fusedPointReactorDebugLevel);
		producedLine = sharedReactor.resultantLine;

	}

	// CASE 3: the host STriangle "engulfed" the guest STriangle; the host STriangle has no points, but the guest will always have 2.
	else if (guestFusionAnalysisRef->fusedPoints.fusedPointMap.size() == 2)
	{
		//std::cout << "(Reactor) Running logic for CASE 2: (Guest) " << std::endl;
		fusedPointReactorLogger.log("(Reactor) Running logic for CASE 2: (Guest) ", "\n");
		FusedPointMetaPair guestPair;
		auto guestFusionPointsBegin = guestFusionAnalysisRef->fusedPoints.fusedPointMap.begin();
		auto guestFusionPointsEnd = guestFusionAnalysisRef->fusedPoints.fusedPointMap.end();
		for (; guestFusionPointsBegin != guestFusionPointsEnd; guestFusionPointsBegin++)
		{
			glm::vec3 currentPointToSearch = guestFusionPointsBegin->second.point;
			FusedPointMeta currentPointMeta = guestFusionAnalysisRef->fusedPoints.retrieveFusedPointMeta(currentPointToSearch, FusionCandidateOrigin::HOST);
			guestPair.insertFusedPointMeta(currentPointMeta);
		}
		GuestLineReactor guestReactor(guestFusionAnalysisRef, &guestPair, fusedPointReactorDebugLevel);
		producedLine = guestReactor.resultantLine;
	}
}

FusedPointReactorResult FusedPointReactor::getReactorResult()
{
	FusedPointReactorResult result;
	result.wasLineProduced = fusionContinuationFlag;
	result.resultingLine = producedLine;
	return result;
}