#include "stdafx.h"
#include "MassZoneBoxBoundarySPolySet.h"

#ifndef SMALL_NUM
#define SMALL_NUM   0.000001 // anything that avoids division overflow
#endif

void MassZoneBoxBoundarySPolySet::setBoundarySPolyRef(SPoly* in_sPolyRef)
{
	boundarySPolyRef = in_sPolyRef;

	//std::cout << ">>> Inserted BoundarySPolyRef; which has " << boundarySPolyRef->borderLines.size() << " Lines. " << std::endl;
	//int stopVal = 3;
	//std::cin >> stopVal;
}

void MassZoneBoxBoundarySPolySet::setLogLevel(PolyDebugLevel in_sPolyDebugLevel)
{
	boxBoundarySPolySetLogLevel = in_sPolyDebugLevel;
	boxBoundarySPolySetLogger.setDebugLevel(in_sPolyDebugLevel);
}

void MassZoneBoxBoundarySPolySet::compareSPolySubZoneSPolyToBoundarySPoly(SPoly* in_sPolyRef)
{
	SPoly* polyA = boundarySPolyRef;

	//std::cout << "!! Empty normal of polyA: " << polyA->polyEmptyNormal.x << ", " << polyA->polyEmptyNormal.y << ", " << polyA->polyEmptyNormal.z << std::endl;

	//std::cout << "!!! Size of triangles: " << polyA->triangles.size() << std::endl;
	//std::cout << "!!! Size of border lines: " << polyA->borderLines.size() << std::endl;
	//std::cout << "!!! Size of border lines, direct: " << boundarySPolyRef->borderLines.size() << std::endl;

	//SPoly dumbPoly;
	//std::cout << "!!! Dumb poly size of border lines: " << dumbPoly.borderLines.size() << std::endl;


	SPoly* polyB = in_sPolyRef;
	//std::cout << ">>> Entering before coplanarity checker. " << std::endl;
	CoplanarChecker checker(polyA, polyB, boxBoundarySPolySetLogger.getLogLevel());
	//std::cout << ">>> Passed coplanarity checker. " << std::endl;

	//int waitVal;
	//std::cin >> waitVal;


	if (checker.coplanarityDetected == false)	// can only compare to a guest sPoly that is non-coplanar to the boundary SPoly.
	{
		//std::cout << " >>> Performing comparison. " << std::endl;
		insertCategorizedLinesFromNonboundarySPoly(in_sPolyRef);
	}
	else if (checker.coplanarityDetected == true)
	{
		//std::cout << "!!!! NOTICE: coplanarity was detected; points are: " << std::endl;
		//std::cout << "::::::::: Border SPoly: " << std::endl;
		//polyA->printPoints();
		//std::cout << "::::::::: Input SPoly: " << std::endl;
		//in_sPolyRef->printPoints();
	}

	if (boxBoundarySPolySetLogger.isLoggingSet() == true)
	{
		polyA->sequenceFactory.printLineCounts();
		polyA->sequenceFactory.printLinesInPool();
		boxBoundarySPolySetLogger.waitForDebugInput();
	}
	//int waitVal;
	//std::cin >> waitVal;
}

void MassZoneBoxBoundarySPolySet::insertCategorizedLinesFromNonboundarySPoly(SPoly* in_guestPolyPtr)
{
	SPoly* in_hostPolyPtr = boundarySPolyRef;
	int in_guestPolyID = currentComparableSPolyIndex;
	int hostPolyTriangleCount = in_hostPolyPtr->numberOfTriangles;
	int guestPolyTriangleCount = in_guestPolyPtr->numberOfTriangles;

	// acquire the debug level for CategorizedLines.
	PolyLogger currentComparisonLogger;
	currentComparisonLogger.setDebugLevel(categorizedLineDebugLevel);
	currentComparisonLogger.log("(MassZoneBoxBoundarySPolySet) Categorized line debug option found.", "\n");


	for (int currentHostPolyTriangle = 0; currentHostPolyTriangle < hostPolyTriangleCount; currentHostPolyTriangle++)					// compare each of poly A's tertiaries...
	{

		//std::cout << "::::::::::::::::::::::::::::::::::: ----------------------------------+++++++++>>>>>>>>>>> Running host poly Triangle comparison: " << std::endl;

		STriangle* hostTrianglePtr = &in_hostPolyPtr->triangles[currentHostPolyTriangle];	// " " 
		IntersectionLineGroup hostLineGroup(currentComparisonLogger.getLogLevel());						// the line group for poly A.
		IntersectionLineGroup guestLineGroup(currentComparisonLogger.getLogLevel());						// the line group for poly B.
		hostLineGroup.setFusionAnalyzerSPolyRef(in_hostPolyPtr);
		guestLineGroup.setFusionAnalyzerSPolyRef(in_guestPolyPtr);
		for (int y = 0; y < guestPolyTriangleCount; y++)					// .. to each of poly B's tertiaries...
		{
			// for each pair (that is, A's current STriangle to B's current STriangle in the iterations), we must:
			// 1. compare the lines of STriangle A to that of STriangle B
			// 2. compare the lines of STriangle B to that of STriangle A
			// 3. analyze the relationship between them

			// >>>>>>>>>>>>>>>>>>>>> STEP 1
			// compare the host triangle lines, to the guest triangles. |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

			STriangle* guestTrianglePtr = &in_guestPolyPtr->triangles[y]; // get the guest poly's triangle
			//std::cout << "::::::::::::::::::::::::::::::::::: (MassZoneBoxBoundarySPolyset) >>>>>>>>>>>>>>>>>>>>>>>>>>> Comparing lines of the host to the guest triangle " << std::endl;
			//std::cout << ">>> B triangle (Guest triangle) points are: " << std::endl;
			//std::cout << "0: " << guestTrianglePtr->triangleLines[0].pointA.x << ", " << guestTrianglePtr->triangleLines[0].pointA.y << ", " << guestTrianglePtr->triangleLines[0].pointA.z << std::endl;
			//std::cout << "1: " << guestTrianglePtr->triangleLines[1].pointA.x << ", " << guestTrianglePtr->triangleLines[1].pointA.y << ", " << guestTrianglePtr->triangleLines[1].pointA.z << std::endl;
			//std::cout << "2: " << guestTrianglePtr->triangleLines[2].pointA.x << ", " << guestTrianglePtr->triangleLines[2].pointA.y << ", " << guestTrianglePtr->triangleLines[2].pointA.z << std::endl;
			//std::cout << "2 (B): " << guestTrianglePtr->triangleLines[2].pointB.x << ", " << guestTrianglePtr->triangleLines[2].pointB.y << ", " << guestTrianglePtr->triangleLines[2].pointB.z << std::endl;
			for (int currentHostTriangleLine = 0; currentHostTriangleLine < 3; currentHostTriangleLine++)		// run the lines of A (the host) through triangle B (the guest)
			{
				//FusionCandidateProducer hostCandidateProducer(currentComparisonLogger.getLogLevel());
				FusionCandidateProducer hostCandidateProducer(PolyDebugLevel::NONE);
				FusionCandidate hostFusedCandidate = hostCandidateProducer.produceCandidate(*guestTrianglePtr, in_hostPolyPtr->triangles[currentHostPolyTriangle].triangleLines[currentHostTriangleLine]);

				if (hostFusedCandidate.candidateIntersectionResult.wasIntersectFound != 0)
				{
					hostLineGroup.insertFusionCandidateIntoAnalyzer(FusionCandidateOrigin::HOST, currentHostTriangleLine, hostFusedCandidate);
				}
			}
			hostLineGroup.runFusionAnalysisAndDetermineClassifications();
			//std::cout << "::::::::::::::::::::::::::::::::::: (MassZoneBoxBoundarySPolyset) >>>>>>>>>>>>>>>>>>>>>>>>>>> ENDED Comparing lines of the host to the guest triangle " << std::endl;
	

			// >>>>>>>>>>>>>>>>>>>>> STEP 2
			// compare the GUEST triangle lines, to the host triangles. ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
			//std::cout << "::::::::::::::::::::::::::::::::::: (MassZoneBoxBoundarySPolyset) >>>>>>>>>>>>>>>>>>>>>>>>>>> Comparing lines of the guest to the host triangle" << std::endl;
			for (int z = 0; z < 3; z++)		// run the lines of B (the guest) through triangle A (the host)
			{
				//FusionCandidateProducer guestCandidateProducer(currentComparisonLogger.getLogLevel());
				FusionCandidateProducer guestCandidateProducer(PolyDebugLevel::NONE);
				FusionCandidate guestFusedCandidate = guestCandidateProducer.produceCandidate(*hostTrianglePtr, guestTrianglePtr->triangleLines[z]);




				if (guestFusedCandidate.candidateIntersectionResult.wasIntersectFound != 0)
				{
					//std::cout << "(MassZoneBoxBoundarySPolySet): (wasIntersectFound value is: " << guestFusedCandidate.candidateIntersectionResult.wasIntersectFound <<  ") guest fused candidate point is: "
						//<< guestFusedCandidate.candidateIntersectionResult.intersectedPoint.x << ", "
						//<< guestFusedCandidate.candidateIntersectionResult.intersectedPoint.y << ", "
						//<< guestFusedCandidate.candidateIntersectionResult.intersectedPoint.z << std::endl;
					guestLineGroup.insertFusionCandidateIntoAnalyzer(FusionCandidateOrigin::GUEST, z, guestFusedCandidate);
				}
			}
			guestLineGroup.runFusionAnalysisAndDetermineClassifications();
			//std::cout << "::::::::::::::::::::::::::::::::::: (MassZoneBoxBoundarySPolyset) >>>>>>>>>>>>>>>>>>>>>>>>>>> ENDED Comparing lines of the guest to the host triangle" << std::endl;

			FusedPointReactor reactor(&hostLineGroup.returnLine.completedAnalysis, &guestLineGroup.returnLine.completedAnalysis, currentComparisonLogger.getLogLevel());
			FusedPointReactorResult reactionResult = reactor.getReactorResult();
			reactionResult.resultingLine.parentPoly = in_guestPolyID;
			if (reactionResult.wasLineProduced == true)
			{
				// if a line was produced, and regardless of whether or not it passes the CategorizedLineColinearTester's check that follows,
				// we must set the lineProductionStain flag. When the MassZoneBox that this instance is a member of checks to see what cube faces must produce tertiary border SPolys,
				// (via a call to produceTertiarySPolys) and IF this instance is one of the faces that must have SPolys, this flag will need to be checked. 
				// If it is set to TRUE, no new SPolys will be returned, because it means that at a minimum, at least one primary SPoly produced a successful candidate line 
				// that may or may not be inserted. The only way it wouldn't be inserted is if the line to insert was colinear to a border line in the SPoly pointed to by boundarySPolyRef, even though its still a valid line.
				// Hence, valid lines dictate that the SPoly that produced the line on this border SPoly are technically valid -- regardless of if they pass the CategorizedLineColinearTester test.
				wasLineProducedByReactor = true;

				CategorizedLineColinearTester tester(reactionResult.resultingLine, *hostTrianglePtr, categorizedLineCoplanarTestsDebugLevel);


				if (tester.colinearToBorderLineDetected == false)		// the categorized line isn't colinear to any line in the host triangle (remember, context is from host triangle)
				{
					//std::cout << "!!!!! Added new line: point A " << reactionResult.resultingLine.line.pointA.x << ", " << reactionResult.resultingLine.line.pointA.y << ", " << reactionResult.resultingLine.line.pointA.z
					//							<< " | point B: " << reactionResult.resultingLine.line.pointB.x << ", " << reactionResult.resultingLine.line.pointB.y << ", " << reactionResult.resultingLine.line.pointB.z << std::endl;

					in_hostPolyPtr->sequenceFactory.addCategorizedLine(reactionResult.resultingLine);
				}
				else if (tester.colinearToBorderLineDetected == true)
				{
					// if contesting the categorized lines is enabled, we need to record it.
					if (isContestedCategorizedLineAnalysisEnabled == true)
					{
						std::cout << "(MassZoneBoxBoundarySPolyset): line with the following points flagged as contested: " << std::endl;
						std::cout << "point A: " << reactionResult.resultingLine.line.pointA.x << ", " << reactionResult.resultingLine.line.pointA.y << ", " << reactionResult.resultingLine.line.pointA.z << std::endl;
						std::cout << "point B: " << reactionResult.resultingLine.line.pointB.x << ", " << reactionResult.resultingLine.line.pointB.y << ", " << reactionResult.resultingLine.line.pointB.z << std::endl;
						contestables.insertChallengingCategorizedLine(tester.idOfColinearBorderLine, reactionResult.resultingLine);
					}
				}
				//numberOfIntersections++;
			}
			
			currentComparisonLogger.log("######>>>>>>> (MassZoneBoxBoundarySPolyset) Comparison complete, enter number to continue to next comparison. ", "\n");
			currentComparisonLogger.waitForDebugInput();

			hostLineGroup.reset();
			guestLineGroup.reset();

			//std::cout << "+++++++++ comparing next tertiary..." << std::endl;
		}

		//std::cout << "------------++ done comparing current tertiary in triangle A; moving to next tertiary in triangle A..." << std::endl;

	}
	
	//std::cout << "::::::::::::::::::: MassZoneBoxBonudarySPolySet: done comparison. " << std::endl;
	//int stopVal = 3;
	//std::cin >> stopVal;
	currentComparableSPolyIndex++;	// this must be incremented, to set the appropriate index for the next SPoly (if there are any to compare against)
}

MessageContainer MassZoneBoxBoundarySPolySet::buildBoundarySPolyFromFactory()
{
	//std::cout << "(MassZoneBoxBoundarySPolySet): begin building cleave sequences..." << std::endl;
	//bool wasBuildSuccessful = true;
	MessageContainer buildErrorMessages;

	buildErrorMessages = boundarySPolyRef->buildCleaveSequences(CleaveSequenceMergeMode::MERGE, boundarySPolySetOrientation);

	//boundarySPolyRef->buildCleaveSequences(CleaveSequenceMergeMode::MERGE);
	//std::cout << "(MassZoneBoxBoundarySPolySet): finished building cleave sequences..." << std::endl;
	if (buildErrorMessages.empty() == true)
	{
		if (boundarySPolyRef->cleaveMap.size() != 0)
		{
			//std::cout << "(MassZoneBoxBoundarySPolyset) !!! Found cleave map values in Factory; processing..." << std::endl;
			SPolyMorphTracker morphTracker;

			//std::cout << "(MassZoneBoxBoundarySPolySet): begin fracturing..." << std::endl;
			SPolyFracturer fracturer(0, boundarySPolyRef, &morphTracker, SPolyFracturerOptionEnum::ROTATE_TO_Z, fracturerDebugLevel);
			//std::cout << "(MassZoneBoxBoundarySPolySet): end fracturing..." << std::endl;
			//std::cout << ">>>>> Size of fracturer SPolySG is: " << fracturer.sPolySG.sPolyMap.size() << std::endl;
			boundarySPolySG = std::move(fracturer.sPolySG);		// should be able to move, since the data in the fracturer's sPolySG is about to be destroyed anyway, once we go out of scope.
			boundarySPolySG.setEmptyNormalInAllSPolys(boundaryEmptyNormal);
			boundarySPolySG.roundAllSTrianglesToHundredths();
			boundarySPolySG.buildSPolyBorderLines();

			//std::cout << "!!! Size of produced SPolys in boundarySPolySG: " << boundarySPolySG.sPolyMap.size() << std::endl;

			//boundarySPolySG.
		}
		//std::cout << "(MassZoneBoxBoundarySPolySet): finished fracturing, and build of border lines..." << std::endl;
		if
		(
			(boundarySPolyRef->cleaveMap.size() == 0)
			&&
			(isContestedCategorizedLineAnalysisEnabled == true)
			&&
			(contestables.contestableBorderLineChallengersMap.size() != 0)
		)
		{
			std::cout << "!!! Running analysis on contestables; will attempt to produce boundary SPolys..." << std::endl;

			std::cout << "Face center point is: " << boundaryFaceCenterPoint.x << ", " << boundaryFaceCenterPoint.y << ", " << boundaryFaceCenterPoint.z << std::endl;
			auto firstContestedLine = contestables.contestableBorderLineChallengersMap.begin()->second.begin();
			std::cout << "First contested categorized line points are: " << firstContestedLine->line.pointA.x << ", " << firstContestedLine->line.pointA.y << ", " << firstContestedLine->line.pointA.z << " | "
				<< firstContestedLine->line.pointB.x << ", " << firstContestedLine->line.pointB.y << ", " << firstContestedLine->line.pointB.z << std::endl;
			std::cout << "First contested categorized line empty normal is: " << firstContestedLine->emptyNormal.x << ", " << firstContestedLine->emptyNormal.y << ", " << firstContestedLine->emptyNormal.z << std::endl;

			requiresContestedAnalysis = true;
			didCategorizedLineWinContest = resolveContest(*firstContestedLine);
			if (didCategorizedLineWinContest == true)
			{
				std::cout << "!! Notice: categorized lie won contest!" << std::endl;
			}
		}
	}
	else if (buildErrorMessages.empty() == false)
	{
		std::cout << "(MassZoneBoxBoundarySPolySet): detected buildCleaveSequence run as being unsuccesful..." << std::endl;
	}
	return buildErrorMessages;
}

bool MassZoneBoxBoundarySPolySet::resolveContest(CategorizedLine in_categorizedLine)
{
	bool didCategorizedLineWin = false;
	CategorizedLine lineCopy = in_categorizedLine;
	glm::vec3 boundaryCenterPointCopy = boundaryFaceCenterPoint;

	// first step: insert the categorized line's points, plus the current center
	QuatRotationPoints contestPoints;
	contestPoints.insertPointRefs(&lineCopy.line.pointA, &lineCopy.line.pointB, &boundaryCenterPointCopy);

	// second step: take point A of the CategorizedLine, translate by it
	PointTranslationCheck pointTranslator;
	pointTranslator.performCheck(lineCopy.line.pointA);
	if (pointTranslator.requiresTranslation == 1)
	{
		contestPoints.applyTranslation(pointTranslator.getTranslationValue());
	}

	// third step: push the empty normal of the categorized line copy only AFTER translation
	contestPoints.insertPointRefs(&lineCopy.emptyNormal);

	didCategorizedLineWin = QuatUtils::resolveContestedCategorizedLine(&contestPoints, PolyDebugLevel::NONE);

	return didCategorizedLineWin;
}