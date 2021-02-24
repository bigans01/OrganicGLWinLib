#include "stdafx.h"
#include "CoplanarRelationships.h"


CoplanarRelationships::CoplanarRelationships()
{

}


//void CoplanarRelationships::setTrackedPolyData(int in_trackedPolyID, SPoly* in_trackedSPolyRef)
void CoplanarRelationships::setTrackedPolyData(int in_trackedPolyID, SPoly in_trackedSPolyRef)
{
	trackedPolyID = in_trackedPolyID;
	trackedSPolyRef = in_trackedSPolyRef;
}
//void CoplanarRelationships::insertRelationship(int in_relatedSPolyID, SPoly* in_relatedSPolyRef)
void CoplanarRelationships::insertRelationship(int in_relatedSPolyID, SPoly in_relatedSPolyRef)
{
	relationshipMap.insertSPolyRef(in_relatedSPolyID, in_relatedSPolyRef);
}

void CoplanarRelationships::setLoggerDebugLevel(PolyDebugLevel in_polyDebugLevel)
{
	relationshipsDebugLevel = in_polyDebugLevel;
	relationshipsLogger.setDebugLevel(in_polyDebugLevel);
}


void CoplanarRelationships::rotateToXYPlaneAndCompare()
{
	// step 1: rotate involved SPolys to the XY plane

	// Printing lines in pool, prior to rotate to Z = 0;
	std::cout << "!##################### ((1)) ! " << std::endl;
	std::cout << "!################### Printing lines for the tracked SPoly with ID: " << trackedPolyID << std::endl;
	//trackedSPolyRef->sequenceFactory.printLinesInPool();
	trackedSPolyRef.sequenceFactory.printLinesInPool();
	std::cout << "!##################### ((2)) ! " << std::endl;
	int someValWaits = 3;
	std::cin >> someValWaits;



	// 1.1: load the points (that is, points of STriangles and SPolyBorderLines) before applying translation.
	// 1.1.1: load points from the trackedSpolyRef
	//trackedSPolyRef->loadTrianglesAndBorderLinesIntoQuatPoints(&coplanarPoints);
	//trackedSPolyRef->loadPrimalsTrianglesAndBordersIntoQuatPoints(&coplanarPoints);
	//trackedSPolyRef->loadAllIntoQuatPoints(&coplanarPoints);		// don't use this, or rework it; (12/17/2020); doing this translates the normals (should NOT be done!!)
	trackedSPolyRef.loadAllIntoQuatPoints(&coplanarPoints);

	// 1.1.2: load points from the related SPolys
	auto relatedSPolysBegin = relationshipMap.refMap.begin();
	auto relatedSPolysEnd = relationshipMap.refMap.end();
	for (; relatedSPolysBegin != relatedSPolysEnd; relatedSPolysBegin++)
	{
		//relatedSPolysBegin->second->loadTrianglesAndBorderLinesIntoQuatPoints(&coplanarPoints);
		relatedSPolysBegin->second.loadTrianglesAndBorderLinesIntoQuatPoints(&coplanarPoints);
	}

	// 1.2: translate the first point of the first triangle in the first SPoly to 0.
	//pointTranslator.performCheck(trackedSPolyRef->borderLines[0].pointA);
	pointTranslator.performCheck(trackedSPolyRef.borderLines[0].pointA);
	if (pointTranslator.requiresTranslation == 1)	// almost 100% of the time, this will be run
	{
		std::cout << "!! prime point 0 requires translation!! " << std::endl;
		//std::cout << "It's value is: " << trackedSPolyRef->borderLines[0].pointA.x << ", " << trackedSPolyRef->borderLines[0].pointA.y << ", " << trackedSPolyRef->borderLines[0].pointA.z << std::endl;
		std::cout << "It's value is: " << trackedSPolyRef.borderLines[0].pointA.x << ", " << trackedSPolyRef.borderLines[0].pointA.y << ", " << trackedSPolyRef.borderLines[0].pointA.z << std::endl;
		coplanarPoints.applyTranslation(pointTranslator.getTranslationValue());
	}
	else
	{
		std::cout << "!! prime point 0 requires no translation. " << std::endl;
	}
	//int numberOfEmptyNormalsInserted = trackedSPolyRef->loadEmptyNormalsIntoQuatPoints(&coplanarPoints);	// normals can only be rotated, not translated; they should be inserted only after 
																		// any translation occurs.
	int numberOfEmptyNormalsInserted = trackedSPolyRef.loadEmptyNormalsIntoQuatPoints(&coplanarPoints);







	// 1.3.1 rotate points by the quaternion, then run them through the STriangleCutter (do not round before using the STriangleCutter!)

	rotationManager.initializeAndRunForZFracture(&coplanarPoints);
	
	std::cout << "+++++++++++++++++ (PRE-TRACKED ROUND TO HUNDREDTHS): printing lines for tracked SPoly: " << std::endl;
	trackedSPolyRef.printBorderLines();
	std::cout << "+++++++++++++++++ (PRE-TRACKED ROUND TO HUNDREDTHS): done printing lines for tracked SPoly. " << std::endl;
	int someVal = 3;
	std::cin >> someVal;

	
	// ########################################### NEW METHOD, to replace below:
	std::cout << "#######################################################" << std::endl;
	std::cout << "#######################################################" << std::endl;
	std::cout << "#######################################################" << std::endl;
	std::cout << "############## BEGIN NEW METHOD TEST for OrganicCore. " << std::endl;
	std::cout << "############## Number of STriangles in trackedCopy to analyze: " << trackedSPolyRef.triangles.size();

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
	std::map<int, bool> cuttingResultsMap;
	auto trackedCopySTrianglesBegin = trackedSPolyRef.triangles.begin();
	auto trackedCopySTrianglesEnd = trackedSPolyRef.triangles.end();
	for (; trackedCopySTrianglesBegin != trackedCopySTrianglesEnd; trackedCopySTrianglesBegin++)
	{
		std::cout << "::::::::::>>>>>>>>>>>> Beginning STriangleCutter attempt for STriangle with ID " << trackedCopySTrianglesBegin->first << std::endl;
		std::cout << "STriangle points are: " << std::endl;
		trackedCopySTrianglesBegin->second.printPoints();
		int readyToContinue = 3;
		std::cin >> readyToContinue;

		STriangleCutter cutter;
		cutter.setCuttingParameters(trackedCopySTrianglesBegin->second, &cuttingManager);
		cutter.runCuttingSequence();

		std::cout << "::::::::::>>>>>>>>>>>> Finished STriangleCutter attempt for STriangle with ID " << trackedCopySTrianglesBegin->first << std::endl;
		std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
		std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
		std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
		std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
		int finishedWait = 3;
		std::cin >> finishedWait;
	}

	std::cout << "############## END NEW METHOD TEST for OrganicCore. " << std::endl;
	std::cout << "#######################################################" << std::endl;
	std::cout << "#######################################################" << std::endl;
	std::cout << "#######################################################" << std::endl;
	int endTest = 3;
	std::cin >> endTest;
	// ########################################### METHOD 1
	



	// 1.3.2: round points, before doing the new SPoly generation.
	coplanarPoints.roundAllPointsToHundredths();

	std::cout << "--> printing lines for tracked SPoly " << std::endl;
	//trackedSPolyRef->printBorderLines();
	trackedSPolyRef.printBorderLines();
	std::cout << "!#####################! " << std::endl;
	//trackedSPolyRef->sequenceFactory.printLinesInPool();
	trackedSPolyRef.sequenceFactory.printLinesInPool();
	std::cout << "!#####################! " << std::endl;

	relatedSPolysBegin = relationshipMap.refMap.begin();
	relatedSPolysEnd = relationshipMap.refMap.end();
	for (; relatedSPolysBegin != relatedSPolysEnd; relatedSPolysBegin++)
	{
		std::cout << "-->printing lines for related SPoly " << std::endl;
		//relatedSPolysBegin->second->printBorderLines();
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
	//if (trackedSPolyRef->massManipulationSetting == MassManipulationMode::CREATION)
	if (trackedSPolyRef.massManipulationSetting == MassManipulationMode::CREATION)
	{
		std::cout << "!!!! MM Mode is set as creation; processing via CoplanarMassCreator..." << std::endl;
		manipulator.reset(new CoplanarMassCreator());
		manipulator->initialize(trackedSPolyRef, relationshipMap, &coplanarPoints, relationshipsDebugLevel);
		manipulator->runMassManipulation();
	}


	// ********************************************** the below steps are deprecated until further notice, as there appears to be no point in rotating the coplanarPoints back to
	// the original positions, since all we're really doing is checking for SPolys that are entirely consumed.

	// 2.2: when the manipulator is done, apply the reverse of the quaternion rotation, and then round back.
	/*
	rotationManager.rotateToOriginalPosition();
	coplanarPoints.roundAllPointsToHundredths();

	// 2.3: before translating back, remove references to the empty normals of the newly produced categorized lines;
	//      we do this by removing point refs beginning from the end of QuatRotationManager::rotationPointsRefVector's pointsRefVector, equivalent to the number
	//      of normals that were passed in (the number which was stored in numberOfEmptyNormalsInserted above).
	rotationManager.eraseElementsFromEndOfPointsRefVector(numberOfEmptyNormalsInserted);

	// 2.4: lastly, translate all involved SPolys back to their original position.
	if (pointTranslator.requiresTranslation == 1)
	{
		coplanarPoints.applyTranslation(pointTranslator.getReverseTranslationValue());
	}
	

	std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
	std::cout << "--> printing lines for tracked SPoly " << std::endl;
	//trackedSPolyRef->printBorderLines();
	trackedSPolyRef.printBorderLines();
	relatedSPolysBegin = relationshipMap.refMap.begin();
	relatedSPolysEnd = relationshipMap.refMap.end();
	for (; relatedSPolysBegin != relatedSPolysEnd; relatedSPolysBegin++)
	{
		std::cout << "-->printing lines for related SPoly " << std::endl;
		//relatedSPolysBegin->second->printBorderLines();
		relatedSPolysBegin->second.printBorderLines();
	}
	*/
}