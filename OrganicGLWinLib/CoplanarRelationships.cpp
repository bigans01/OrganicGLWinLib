#include "stdafx.h"
#include "CoplanarRelationships.h"

void CoplanarRelationships::setTrackedPolyData(int in_trackedPolyID, SPoly in_trackedSPoly)
{
	trackedPolyID = in_trackedPolyID;
	trackedSPoly = in_trackedSPoly;
}

void CoplanarRelationships::insertRelationship(int in_relatedSPolyID, SPoly in_relatedSPolyRef)
{
	relationshipMap.insertSPolyRef(in_relatedSPolyID, in_relatedSPolyRef);
}

void CoplanarRelationships::setLoggerDebugLevel(PolyDebugLevel in_polyDebugLevel)
{
	relationshipsDebugLevel = in_polyDebugLevel;
	relationshipsLogger.setDebugLevel(in_polyDebugLevel);
}

bool CoplanarRelationships::performCuttingSequenceTest()
{
	bool didSPolySurvive = true;

	// ########################################### NEW METHOD, to replace below:
	std::cout << "#######################################################" << std::endl;
	std::cout << "#######################################################" << std::endl;
	std::cout << "#######################################################" << std::endl;
	std::cout << "############## BEGIN NEW METHOD TEST for OrganicCore. " << std::endl;
	std::cout << "############## Number of STriangles in trackedCopy to analyze: " << trackedSPoly.triangles.size();

	CuttingTriangleManager cuttingManager;
	// load all STriangles that aren't in the tracked copy, into the cuttingManager.
	auto relatedSPolysToUseForCuttingBegin = relationshipMap.refMap.begin();
	auto relatedSPolysToUseForCuttingEnd = relationshipMap.refMap.end();
	for (; relatedSPolysToUseForCuttingBegin != relatedSPolysToUseForCuttingEnd; relatedSPolysToUseForCuttingBegin++)
	{
		auto currentSTrianglesBegin = relatedSPolysToUseForCuttingBegin->second.triangles.begin();
		auto currentSTrianglesEnd = relatedSPolysToUseForCuttingBegin->second.triangles.end();
		for (; currentSTrianglesBegin != currentSTrianglesEnd; currentSTrianglesBegin++)
		{
			cuttingManager.insertCuttingTriangle(currentSTrianglesBegin->second);
		}
	}

	std::cout << "############################# Printing out CuttingTriangles that will be used: " << std::endl;
	cuttingManager.printCuttingTriangles();

	int beginTest = 3;
	std::cin >> beginTest;

	// run each STriangle in the tracked copy, against the STriangles in the cuttingManager.
	STriangleCutter cutter;
	std::map<int, bool> sTriangleDestructionTrackerMap;	// used for tracking whether or not each STriangle in the tracked copy was completely destroyed.
	auto trackedCopySTrianglesBegin = trackedSPoly.triangles.begin();
	auto trackedCopySTrianglesEnd = trackedSPoly.triangles.end();
	for (; trackedCopySTrianglesBegin != trackedCopySTrianglesEnd; trackedCopySTrianglesBegin++)
	{
		std::cout << "::::::::::>>>>>>>>>>>> Beginning STriangleCutter attempt for STriangle with ID " << trackedCopySTrianglesBegin->first << std::endl;
		std::cout << "STriangle points are: " << std::endl;
		trackedCopySTrianglesBegin->second.printPoints();
		int readyToContinue = 3;
		std::cin >> readyToContinue;

		STriangleCutter cutter;
		cutter.setCuttingParameters(trackedCopySTrianglesBegin->second, &cuttingManager);
		sTriangleDestructionTrackerMap[trackedCopySTrianglesBegin->first] = cutter.runCuttingSequence();

		std::cout << "::::::::::>>>>>>>>>>>> Finished STriangleCutter attempt for STriangle with ID " << trackedCopySTrianglesBegin->first << std::endl;
		std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
		std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
		std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
		std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
		int finishedWait = 3;
		std::cin >> finishedWait;
	}

	// now, check if each destruction flag in sTriangleDestructionTrackerMap was set to TRUE. If this is the case, it means
	// all STriangles were completely destroyed, which means the SPoly itself was completely destroyed.
	int destroyedSTriangleCount = 0;
	auto destructionMapBegin = sTriangleDestructionTrackerMap.begin();
	auto destructionMapEnd = sTriangleDestructionTrackerMap.end();
	for (; destructionMapBegin != destructionMapEnd; destructionMapBegin++)
	{
		if (destructionMapBegin->second == true)
		{
			destroyedSTriangleCount++;
		}
	}
	if (destroyedSTriangleCount == sTriangleDestructionTrackerMap.size())
	{
		std::cout << "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| " << std::endl;
		std::cout << "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| " << std::endl;
		std::cout << "||||||||||||||||||||||||||||||| NOTICE, SPoly was completely destroyed. " << std::endl;
		std::cout << "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| " << std::endl;
		std::cout << "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| " << std::endl;
		didSPolySurvive = false;
	}

	std::cout << "############## END NEW METHOD TEST for OrganicCore. " << std::endl;
	std::cout << "#######################################################" << std::endl;
	std::cout << "#######################################################" << std::endl;
	std::cout << "#######################################################" << std::endl;
	int endTest = 3;
	std::cin >> endTest;

	return didSPolySurvive;
}


bool CoplanarRelationships::rotateToXYPlaneAndRunCuttingSequenceTests()
{
	// Step 1: rotate involved SPolys to the XY plane

	// Printing lines in pool, prior to rotate to Z = 0;
	std::cout << "!##################### ((1)) ! " << std::endl;
	std::cout << "!################### Printing lines for the tracked SPoly with ID: " << trackedPolyID << std::endl;
	//trackedSPoly->sequenceFactory.printLinesInPool();
	trackedSPoly.sequenceFactory.printLinesInPool();
	std::cout << "!##################### ((2)) ! " << std::endl;
	int someValWaits = 3;
	std::cin >> someValWaits;



	// Step 2: load the points (that is, points of STriangles and SPolyBorderLines) before applying translation.
	// 2.1: load points from the trackedSPoly
	trackedSPoly.loadAllIntoQuatPoints(&coplanarPoints);

	// 2.2: load points from the related SPolys
	auto relatedSPolysBegin = relationshipMap.refMap.begin();
	auto relatedSPolysEnd = relationshipMap.refMap.end();
	for (; relatedSPolysBegin != relatedSPolysEnd; relatedSPolysBegin++)
	{
		//relatedSPolysBegin->second->loadTrianglesAndBorderLinesIntoQuatPoints(&coplanarPoints);
		relatedSPolysBegin->second.loadTrianglesAndBorderLinesIntoQuatPoints(&coplanarPoints);
	}

	// Step 3: translate the first point of the first triangle in the first SPoly to 0, if we need to.
	pointTranslator.performCheck(trackedSPoly.borderLines[0].pointA);
	if (pointTranslator.requiresTranslation == 1)	// almost 100% of the time, this will be run
	{
		//std::cout << "!! prime point 0 requires translation!! " << std::endl;
		//std::cout << "It's value is: " << trackedSPoly.borderLines[0].pointA.x << ", " << trackedSPoly.borderLines[0].pointA.y << ", " << trackedSPoly.borderLines[0].pointA.z << std::endl;
		coplanarPoints.applyTranslation(pointTranslator.getTranslationValue());
	}

	/*
	// Load the empty normals from the tracked SPoly.
	// The normals should only be rotated, not translated; they should be inserted only after 
	// any translation occurs.
	//trackedSPoly.loadEmptyNormalsIntoQuatPoints(&coplanarPoints);		// |||||||| Flagged as deprecated, 2/25/2021; only used with the commented out deprecated code-block below.
	*/

	// Step 4: Rotate points by the quaternion, then run them through the cutting sequence test (do NOT round before running this test!)
	rotationManager.initializeAndRunForZFracture(&coplanarPoints);
	/*
	std::cout << "+++++++++++++++++ (PRE-TRACKED ROUND TO HUNDREDTHS): printing lines for tracked SPoly: " << std::endl;
	trackedSPoly.printBorderLines();
	std::cout << "+++++++++++++++++ (PRE-TRACKED ROUND TO HUNDREDTHS): done printing lines for tracked SPoly. " << std::endl;
	int someVal = 3;
	std::cin >> someVal;
	*/
	bool didSPolyPassCuttingSequenceTests = performCuttingSequenceTest();		
	/*

	//||||||||||||||||||||||||||||||||||||||||||||||| Flagged as deprecated, on 2/25/2021, replaced by functionality of the 
	//||||||||||||||||||||||||||||||||||||||||||||||| CoplanarRelationships::performCuttingSequenceTest().
	//||||||||||||||||||||||||||||||||||||||||||||||| This section of code is deemed "historical" and may be reviewed in the future,
	//||||||||||||||||||||||||||||||||||||||||||||||| running it may or may not cause a program crash and it should only be used for
	//||||||||||||||||||||||||||||||||||||||||||||||| debugging purposes.

	// only attempt to build border lines, if the SPoly passed the cutting sequence test.
	// If it didn't pass, there is no point in doing the calculations, as it is going to be removed.

	if (didSPolyPassCuttingSequenceTests == true)
	{
		// 1.3.2: round points, before doing the new SPoly generation.
		coplanarPoints.roundAllPointsToHundredths();

		std::cout << "--> printing lines for tracked SPoly " << std::endl;
		trackedSPoly.printBorderLines();
		std::cout << "!#####################! " << std::endl;
		trackedSPoly.sequenceFactory.printLinesInPool();
		std::cout << "!#####################! " << std::endl;

		relatedSPolysBegin = relationshipMap.refMap.begin();
		relatedSPolysEnd = relationshipMap.refMap.end();
		for (; relatedSPolysBegin != relatedSPolysEnd; relatedSPolysBegin++)
		{
			std::cout << "-->printing lines for related SPoly " << std::endl;
			relatedSPolysBegin->second.printBorderLines();
		}

		std::cout << "Pre-rotate print out complete; continue? " << std::endl;
		int preRotate;
		std::cin >> preRotate;

		// step 2: check if its MassManipulationMode::CREATION or DESTRUCTION.
		//		if CREATION -> use CoplanarMassCreator
		//		if DESTRUCTION -> use CoplanarMassDestroyer
		// arguments:
		// 1.) a reference to the tracked SPoly
		// 2.) a copy of the relationship map
		// 3.) a referenece to this instance's instantiaton of coplanarPoints. This is because we will first append the new CategorizedLines produced by the 
		// CoplanarMassManipulator, apply the quaternion to them, then remove the references to the empty normal, before translating the points of the categorized lines.
		// All of this data should be insreted at the end of the coplanarPoints.

		// 2.1: set the manipulator, run as CREATION or DESTRUCTION after initializing.
		//if (trackedSPoly->massManipulationSetting == MassManipulationMode::CREATION)
		if (trackedSPoly.massManipulationSetting == MassManipulationMode::CREATION)
		{
			std::cout << "!!!! MM Mode is set as creation; processing via CoplanarMassCreator..." << std::endl;
			manipulator.reset(new CoplanarMassCreator());
			manipulator->initialize(trackedSPoly, relationshipMap, &coplanarPoints, relationshipsDebugLevel);
			manipulator->runMassManipulation();
		}
	}
	*/
	return didSPolyPassCuttingSequenceTests;
}